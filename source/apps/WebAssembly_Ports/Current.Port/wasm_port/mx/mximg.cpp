/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/




/* mximg v1 www.lostsidedead.com via GPL */



#include "mximg.h"
#include<cassert>
#include<cstring>
#include<cstdio>


namespace mx
{

	mximg_Item::mximg_Item() {
		compressed_length = 0;
		data_length = 0;
		name_length = 0;
		offset = 0;
	}

	mximg_Writer::mximg_Writer() {


	}

	bool mximg_Writer::addItem(const char *item) {

		std::fstream f;
		f.open(item, std::ios::in | std::ios::binary);
		if(!f.is_open())
			return false;
		f.seekg(0, std::ios::end);
		size_t len = f.tellg();
		f.close();

		if(len == 0) {
			std::cout << item << " is zero bytes, skipping..\n";
			return true;
		}


		mximg_Item it;
		it.data_length = len;

		it.fullname = item;

		std::string ex = item;
		if(ex.rfind("/")) {
			ex = ex.substr(ex.rfind("/")+1, ex.length());
			std::cout << ex << "\n";
		}

		it.name_length = ex.length()+1;
		snprintf(it.name, it.name_length, "%s", ex.c_str());

		return addItem(it);
	}

	bool mximg_Writer::addItem(const mximg_Item &item) {

		assert(item.data_length != 0);
		assert(item.name_length != 0);

		items_.push_back(item);
		return true;
	}

	bool mximg_Writer::writeData(std::fstream &file) {

		std::vector<mximg_Item>::iterator i, last;

		for(i = items_.begin(), last = items_.end(); i != last; i++) {

			if(i->name_length <= 0) continue;
			if(i->data_length <= 0) continue;


			file.write((char*)&i->name_length, sizeof(unsigned int));
			file.write((char*)i->name, i->name_length);

			unsigned char *temp_bytes = new unsigned char [ i->data_length + 1 ];
			std::fstream temp_file;
			temp_file.open(i->fullname.c_str(), std::ios::in | std::ios::binary);
			if(!temp_file.is_open()) {
				std::cerr << " could not open input file: " << i->name << "\n";
				file.close();
				return false;
			}
			temp_file.read((char*)temp_bytes, i->data_length);
			temp_file.close();
			i->compressed_length = i->data_length;

			int err;

			i->compressed_length = compressBound(i->data_length);
			unsigned char *comp_bytes = new unsigned char [ i->compressed_length + 1 ];

			if((err = compress((Bytef*)comp_bytes, (uLongf*)&i->compressed_length, (const Bytef*)temp_bytes, (uLong) i->data_length)) != Z_OK) {
				file.close();
				delete [] temp_bytes;
				delete [] comp_bytes;
				std::cerr << "error on compression of " << i->name <<  " " << zError(err) << " ...\n";
				return false;
			}

			file.write((char*)&i->data_length, sizeof(unsigned int));
			file.write((char*)&i->compressed_length, sizeof(unsigned int));
			file.write((char*)comp_bytes, i->compressed_length);

			delete [] temp_bytes;
			delete [] comp_bytes;
		}

		file.close();

		return true;
	}


	bool mximg_Writer::concatItems(const char *filen) {

		std::fstream file;

		file.open(filen, std::ios::binary | std::ios::app | std::ios::out);
		if(!file.is_open()) return false;

		return writeData(file);
	}

	bool mximg_Writer::writeItems(const char *filen)
	{
		std::fstream file;
		file.open(filen, std::ios::out | std::ios::binary);
		if(!file.is_open())
			return false;

		return writeData(file);
	}

	mximg_Reader::mximg_Reader() {}


 	bool mximg_Reader::openArchive(const char *filen) {

 		std::fstream f;

 		f.open(filen, std::ios::in | std::ios::binary);

 		if(!f.is_open()) return false;

 		while(!f.eof() ) {

 			mximg_Item item;
 			f.read((char*)&item.name_length, sizeof(unsigned int));
 			f.read((char*)&item.name, item.name_length);
 			f.read((char*)&item.data_length, sizeof(unsigned int));
 			f.read((char*)&item.compressed_length, sizeof(unsigned int));
 			item.offset = f.tellg();
 			f.seekg(item.compressed_length, std::ios::cur);

 			if(item.compressed_length != 0)
 			items_.push_back(item);
 		}
 		f.close();
 		file_name = filen;
		return true;
	}

	void mximg_Reader::listItems(std::ostream &stream) {

		std::vector<mximg_Item>::iterator i, last;

		using namespace std;

		stream << setw(25) << "filename" << setw(15) << "compressed" << setw(15) << "uncompressed" << "\n";

		for(i = items_.begin(), last = items_.end(); i != last; i++) {
			stream << setw(25) <<  i->name << setw(15) << i->compressed_length << setw(15) << i->data_length << "\n";
		}
	}

	void *mximg_Reader::accessBytes(const char *fname, size_t &length) {

		std::vector<mximg_Item>::iterator i,last;

		for(i = items_.begin(), last = items_.end(); i != last; i++) {

			if(strcmp(i->name, fname) == 0) {


				std::fstream f;
				f.open(file_name.c_str(), std::ios::in | std::ios::binary);
				if(!f.is_open()) return 0;

				length = i->data_length;
				unsigned char *compressed_bytes = new unsigned char [ i->compressed_length + 1 ];
				unsigned char *uncompressed = new unsigned char [ i->data_length + 1 ];

				f.seekg(i->offset, std::ios::beg);
				f.read((char*)compressed_bytes, i->compressed_length);
				f.close();
				if( uncompress((Bytef*)uncompressed, (uLongf*)&i->data_length,(const Bytef*)compressed_bytes, (uLongf)i->compressed_length) != Z_OK ) {

					std::cerr << "error on decompression\n";
					delete [] uncompressed;
					delete [] compressed_bytes;
					return 0;
				}

				delete [] compressed_bytes;
				return uncompressed;
			}

		}
		return 0;
	}

	void mximg_Reader::extractItem(const char *path, const char *fname) {

		std::string p = path;
		if(p[p.length()-1] == '/') p += fname; else p = p + '/' + fname;

		std::fstream file;
		file.open(p.c_str(), std::ios::out | std::ios::binary);
		if(!file.is_open()) {
			std::cerr << "error path is incorrect, supply correct path for extraction.";
			return;
		}

		size_t length = 0;
		char *bytes = (char*) accessBytes(fname, length);
		if(bytes == 0) std::cerr << "invalid identifier for extraction.";

		file.write((char*)bytes, length);
		file.close();
		delete [] bytes;

	}

	void mximg_Reader::extractAll(const char *path) {

		std::vector<mximg_Item>::iterator i, last;
		for(i = items_.begin(), last = items_.end(); i != last; i++)
				extractItem(path, i->name);

	}

	void mximg_Reader::closeArchive() {
		if(!items_.empty())
		items_.clear();
	}










}












