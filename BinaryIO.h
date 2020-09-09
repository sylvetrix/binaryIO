#ifndef __BINARYIO_H__
#define __BINARYIO_H__

// #include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#define byte unsigned char

// using std::byte;
using std::fstream;
using std::ios;
using std::string;
using std::vector;

enum Endian {
    Big,
    Little,
};

enum BinaryIOError {
	None,
	GenericReadError,
	GenericWriteError,
	CannotOpenFile,
	FileDoesNotExist,
	NotEnoughData,
};

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
static const Endian endian = Little;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
static const Endian endian = Big;
#else
static const Endian endian = Little;
#endif

class BitConverter {
	public:
		static bool isLittleEndian();
		static void forceSetEndian(Endian newEndian);
		static void forceUnsetEndian();
		// to bytes
		static vector<byte> getBytes(bool value);
		static vector<byte> getBytes(char value);
		static vector<byte> getBytes(signed char value);
		static vector<byte> getBytes(unsigned char value);
		static vector<byte> getBytes(float value);
		static vector<byte> getBytes(double value);
		// static vector<byte> getBytes(int8_t value);
		static vector<byte> getBytes(int16_t value);
		static vector<byte> getBytes(int32_t value);
		static vector<byte> getBytes(int64_t value);
		// static vector<byte> getBytes(uint8_t value);
		static vector<byte> getBytes(uint16_t value);
		static vector<byte> getBytes(uint32_t value);
		static vector<byte> getBytes(uint64_t value);
		// from bytes
		static bool getBool(vector<byte> bytes);
		static char getChar(vector<byte> bytes);
		static signed char getSChar(vector<byte> bytes);
		static unsigned char getUChar(vector<byte> bytes);
		static float getFloat(vector<byte> bytes);
		static double getDouble(vector<byte> bytes);
		static int8_t getInt8(vector<byte> bytes);
		static int16_t getInt16(vector<byte> bytes);
		static int32_t getInt32(vector<byte> bytes);
		static int64_t getInt64(vector<byte> bytes);
		static uint8_t getUInt8(vector<byte> bytes);
		static uint16_t getUInt16(vector<byte> bytes);
		static uint32_t getUInt32(vector<byte> bytes);
		static uint64_t getUInt64(vector<byte> bytes);

	private:
		// to bytes
		static vector<byte> getBytes1(uint8_t value);
		static vector<byte> getBytes2(uint16_t value);
		static vector<byte> getBytes4(uint32_t value);
		static vector<byte> getBytes8(uint64_t value);
		// from bytes
		static uint8_t getValue1(vector<byte> bytes);
		static uint16_t getValue2(vector<byte> bytes);
		static uint32_t getValue4(vector<byte> bytes);
		static uint64_t getValue8(vector<byte> bytes);

		static bool forceEndian;
		static Endian endianOverride;
};

class BinaryIOBase {
	public:
		BinaryIOBase(string fileLocation, ios::openmode mode);
		~BinaryIOBase();
		bool hasError();
		BinaryIOError getError();
		void forceSetEndian(Endian endian);
		void forceUnsetEndian();

	protected:
		bool isLittleEndian();
		ios::openmode mode;
		string fileLocation;
		fstream stream;
		static const int BUFFERMAX = 16384;
		int bufferPos = 0, bufferDataSize = 0;
		char buffer[BUFFERMAX] = { };
		BinaryIOError lastError;

	private:
		bool forceEndian;
		Endian endianOverride;
};

class BinaryReader : public BinaryIOBase {
	public:
		BinaryReader(const char* fileLocation);
		BinaryReader(string fileLocation);
		~BinaryReader();
		bool moreData();
		bool readBool();
		byte readByte();
		char readChar();
		signed char readSChar();
		unsigned char readUChar();
		float readFloat();
		double readDouble();
		int8_t readInt8();
		int16_t readInt16();
		int32_t readInt32();
		int64_t readInt64();
		uint8_t readUInt8();
		uint16_t readUInt16();
		uint32_t readUInt32();
		uint64_t readUInt64();
		vector<byte> readBytes(int count);

	private:
		uint8_t read1();
		uint16_t read2();
		uint32_t read4();
		uint64_t read8();
		void readNextChunk();
};

class BinaryWriter : public BinaryIOBase {
	public:
		BinaryWriter(const char* fileLocation, bool overwrite = false);
		BinaryWriter(string fileLocation, bool overwrite = false);
		~BinaryWriter();
		void write(bool value);
		// void write(byte value);
		void write(char value);
		void write(signed char value);
		void write(unsigned char value);
		void write(float value);
		void write(double value);
		// void write(int8_t value);
		void write(int16_t value);
		void write(int32_t value);
		void write(int64_t value);
		// void write(uint8_t value);
		void write(uint16_t value);
		void write(uint32_t value);
		void write(uint64_t value);
		void write(vector<byte> bytes);
		void write(vector<byte> bytes, int start, int count);

	private:
		void write1(uint8_t value);
		void write2(uint16_t value);
		void write4(uint32_t value);
		void write8(uint64_t value);
		void flush();
};

#endif // __BINARYIO_H__
