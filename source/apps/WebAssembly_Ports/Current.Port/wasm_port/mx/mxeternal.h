/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef _MX_ETERN_H_
#define _MX_ETERN_H_

#include<map>
#include<string>
#include<fstream>
#include<iostream>


namespace mx
{

	using std::map;
	using std::fstream;


	template<typename Type> int defaultRead(fstream &fobj, Type &typeobject)
	{
		fobj.read((char*)&typeobject, sizeof(typeobject));
		return 0;
	}

	template<typename Type> int defaultWrite(fstream &fobj, Type &typeobject)
	{
		fobj.write((char*)&typeobject, sizeof(typeobject));
		return 0;
	}
	// the 'eternal' pool of variables read/write object
	// pass it two functions, one for reading and one for writing
	// the default will work for built in types
	// all memory allocated on the free store in unique types
	// should have its own read/write function

	template<typename T>
	class mxEternal;


	template<typename Type>
	/*! \class Eternal class reader/write Object for persistence of objects */
	class mxEternalRW  {
	public:

		/*! default constructor with default read/write object
		 *
		 * \param rf read function template if none specified default is used
		 * \param wf write function template if none specified default is used
		 */
		mxEternalRW(int (*rf)(fstream &fobj, Type &typeobject) = defaultRead, int (*wf)(fstream &fobj, Type &typeobject) = defaultWrite)
		{
			readFunction = rf;
			writeFunction = wf;
		}
		virtual ~mxEternalRW() { }
	private:
		Type typedata;
		int readType(fstream &fobj);
		int writeType(fstream &fobj);

		int (*readFunction)(fstream &fobj, Type &typeobject);
		int (*writeFunction)(fstream &fobj, Type &typeobject);

		friend class mxEternal<Type>;
	};

	// Eternal pool of auto-reloading varaibles
	template<typename Type>
	/*! \class mxEternal class for persistence of objects */
	class  mxEternal  {
	public:
		/*! mxEternal default constructor
		 * \param name name of file to write object stream to
		 * \param typeobj read/write object containing object read/write functions
		 */
		mxEternal(std::string name="etern.mxdb", mxEternalRW<Type> *typeobj = new mxEternalRW<Type>);
		virtual ~mxEternal(); // important virutal destructor

		/*! setStreamRW set the current read/write stream
		 * \param typeobjx current stream object pointer
		 */
		void setStreamRW(mxEternalRW<Type> *typeobjx); // manualy set the Read/Write Object
		/*! read the database from the file specified in constructor
		 * \return integer for success of operation
		 *  */
		int read(); // reload the db
		/*! write teh database to file specified in constructor
		 * \return integer for success of operation
		 */
		int save(); // save   the db

		/*! get the name of the database file
		 * \return string of the name of database
		 *  */
		const std::string getName() const { return name; } // get this DB filename
		// allow manipulation of the map through this classes interface


		/*! overloaded [] operator for access object in array by n
		 * \param n string for array access
		 * \return array variable index of type specified in template parameter
		 */
		Type &operator[](std::string n) { return this_map[n]; }

		/*!
		 * overloaded -> operator
		 * \return pointer to map of variables of type Type
		 */
		map<std::string, Type> *operator->() { return &this_map; } // use operator -> to access the maps member functions

		/*! typedef */
		typedef typename std::map<std::string, Type>::iterator iterator;

	private:


		std::string name;
		std::fstream fobj,fobj_write;
		Type typedata;

		mxEternalRW<Type> *typeobj;
		std::map<std::string, Type> this_map;

		/*! readType */
		int readType();
		/*! writeType */
		int writeType();

		mxEternal(mxEternal<Type> &t);
		mxEternal<Type>& operator=(const mxEternal<Type> &type);
	};


	template<typename Type> std::ostream& operator<<(std::ostream &out, mxEternal<Type> &type);


	// default reader / writer
	// can be replaced by a custom for unique data type

	// the default will work with built in types and structures which contain only bulit in types.
	// all memory allocated on the freestore will require a custom Reader/Writer.

	template<typename Type>
	int mxEternalRW<Type>::readType(fstream &fobj)
	{
		//fobj.read((char*)&typedata, sizeof(typedata));

		readFunction(fobj, typedata);
		return 0;
	}

	template<typename Type>
	int mxEternalRW<Type>::writeType(fstream &fobj)
	{
		//fobj.write((char*)&typedata, sizeof(typedata));
		writeFunction(fobj, typedata);
		return 0;
	}

	template<typename Type> mxEternal<Type>::mxEternal(std::string vname, mxEternalRW<Type> *typeobject)
	{
		typeobj = typeobject;
		name = vname;
		read();
	}

	template<typename Type> void mxEternal<Type>::setStreamRW(mxEternalRW<Type> *typeobjx)
	{
		if(typeobj != 0) delete typeobj;
		typeobj = 0;
		typeobj = typeobjx;
	}

	template<typename Type> mxEternal<Type>::~mxEternal()
	{
		save();
		if(this->typeobj != 0) delete typeobj;
		this_map.erase(this_map.begin(), this_map.end());
	}

	template<typename Type> int mxEternal<Type>:: read()
	{

		if(!this_map.empty())  this_map.erase(this_map.begin(), this_map.end());

		fobj.open(name.c_str(), std::ios::in | std::ios::binary);
		if(!fobj.is_open()) return 1; // no db or removed start over

		unsigned int number_of_types = 0;

		fobj.read((char*)&number_of_types, sizeof(number_of_types));

		for(unsigned int i = 0; (i < number_of_types) && (!fobj.eof()); i++)
		{
			unsigned int name_index = 0;
			fobj.read((char*)&name_index, sizeof(name_index));

			if(static_cast<int>(name_index) < 0) return -1;

			char *buf = 0;

			try
			{

				buf = new char [ name_index+1 ];
				fobj.read((char*)buf, name_index);
				buf[name_index] =  0;
				readType();
				this_map[buf] = typedata;
				delete [] buf;
				buf = 0;

			}
			catch (...)
			{

				if(buf != 0) delete [] buf;

			}

		}
		fobj.close();
		return 0;
	}
	template<typename Type> int mxEternal<Type>::save()
	{

		fobj_write.open(name.c_str(), std::ios::out | std::ios::binary);
		if(!fobj_write.is_open()) return -1;

		unsigned int length = this_map.size();

		fobj_write.write((char*)&length, sizeof(length));

		typename mxEternal<Type>::iterator i;

		for(i= this_map.begin(); i!=this_map.end(); i++)
		{

			unsigned int len =i->first.length();
			const char *str = i->first.c_str();
			fobj_write.write((char*)&len, sizeof(len));
			fobj_write.write((char*)str,len);
			typedata = i->second;
			writeType();
		}

		fobj_write.close();
		return 0;

	}

	// use RW object to read/write from stream
	// create a new RW object for unique types
	template<typename Type> int mxEternal<Type>::readType()
	{
		typeobj->readType(fobj);
		typedata = typeobj->typedata;
		return 0;
	}

	template<typename Type> int mxEternal<Type>::writeType()
	{
		typeobj->typedata = typedata;
		typeobj->writeType(fobj_write);
		return 0;
	}



	template<typename Type> std::ostream& operator<<(std::ostream &out, mxEternal<Type> &type)
	{


		typename mxEternal<Type>::iterator  i;
		int counter = 0;
		out << " eternal map (" << type.getName() << ") = { \n";

		for(i = type->begin(); i != type->end(); counter++)
		{
			out << "[" << "\"" << i->first << "\"" << " : ";
			out << (i->second);
			out << " ] ";
			i++;
			if(i != type->end()) out << ",\n";
		}
		out << "\n}\n";
		return out;
	}

}
#endif



