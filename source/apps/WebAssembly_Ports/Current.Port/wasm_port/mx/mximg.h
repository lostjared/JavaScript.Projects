/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



/* mximg - v1 www.lostsidedead.com via GPL */

#ifndef _MX_IMG_H_
#define _MX_IMG_H_

#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<vector>
#include<zlib.h>


namespace mx
{


	/*! \class mximg_Item mximg_Item is a class used by mximg_Reader and mximg_Writer
	 * it holds key variables for compressing and uncompressing file data
	 */
	class mximg_Item {
	public:
		/*! constructor */
		mximg_Item();
		/*! all self described variables related to the compressed file: */
		unsigned int compressed_length, data_length, name_length, offset;
		/*! the name of the item in question */
		char name[255];
		/*! the identifier to hold the whole path name for mximg_Writer */
		std::string fullname;
	};


	/*! \class mximg_Writer class to create .mximg files */
	class mximg_Writer {
		mximg_Writer(const mximg_Writer &);
		mximg_Writer& operator=(const mximg_Writer &);
	public:
		/*! default constructor creates object */
		mximg_Writer();
		/*!
		 *  addItem, adds a file to the list of files to be compressed
		 * \param item filename of item to be padded
		 * \return boolean variable to stand for success of the add
		 */
		bool addItem(const char *item);
		/*! addItem
		 * \param item item of type mximg_Item to be added
		 *  \return boolean variable to stand for success of add
		 */
		bool addItem(const mximg_Item &item);
		/*! writeItems
		 *  write the items stored in the mximg_Items vector to file compressed
		 *  \param filen filename of compressed file
		 *  \return boolean variable to stand for success of compression
		 */
		bool writeItems(const char *filen);
		/*! concatItems
		 *  append the items to a existing mximg archive
		 *  \param filen name of archive to append to
		 */
		bool concatItems(const char *filen);
	protected:
		/*! vector of mximg_Item 's */
		std::vector<mximg_Item> items_;
		/*! protected function, write current items in items_ to fstream object f
		 * \param f fstream object
		 * \return boolean value for success?
		 */
		bool writeData(std::fstream &f);
	};


	/*! \class mximg_Reader
	 *  class to encapsulate reading of mximg files and load contents of files
	 *  into memory uncompressed
	 */
	class mximg_Reader {
		mximg_Reader(const mximg_Reader &);
		mximg_Reader& operator=(const mximg_Reader &);
	public:
		/*! default constructor, builds object */
		mximg_Reader();
		/*! opens the archive
		 * \param filen filename of archive
		 * \return boolean value for success
		 */
		bool openArchive(const char *filen);

		/*!
		 * accessBytes access the raw uncompressed bytes of file stored in archive
		 * \param fname filename of data to be loaded and uncompressed and returned
		 * \param length reference to a  unsigned integer to hold the length of the data
		 * \return void * containing the location in memory where the uncompressed bytes are stored.
		 */
		void *accessBytes(const char *fname, size_t &length);
		/*!
		 * listItems list index of file's compressed in archive
		 * \param stream ostream object for output to be copied to.
		 */
		void listItems(std::ostream &stream);
		/*!
		 * extractItem
		 * will extract a item from the archive to a file on file system
		 * \param path filename in archive,
		 * \param fname filename on disk
		 */
		void extractItem(const char *path, const char *fname);
		/*!
		 * extractAll
		 * will extract All of the archive to path
		 * \param path path to extract to
		 */
		void extractAll(const char *path);
		/*!
		 *  closeArchive
		 * will close the currently open archive
		 */
		void closeArchive();
	protected:
		/*! items vector of mximg_Item structures for items in archive */
		std::vector<mximg_Item> items_;
		/*! filename of currently open archive */
		std::string file_name;
	};
}














#endif


