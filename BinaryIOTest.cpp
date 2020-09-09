#include <algorithm>
#include <iomanip>
#include <sstream>

#include "BinaryIO.h"
#include "Logger.h"

using std::ifstream;
using std::ofstream;

#define BINARYIO_LOG "BinaryIOTest.log"
// Little endian files
#define TEST_WRITELE "TestWriteLE.bin"
#define TEST_STATICLE "TestStaticLE.bin"
// Big endian files
#define TEST_WRITEBE "TestWriteBE.bin"
#define TEST_STATICBE "TestStaticBE.bin"

#define TEST_VALUECOUNT 29
#define TEST_BYTECOUNT 93

enum TestValueType {
	Bool,
	Byte,
	Char,
	SChar,
	UChar,
	Float,
	Double,
	Int8,
	Int16,
	Int32,
	Int64,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
};

union TestValueRaw {
	bool vBool;
	byte vByte;
	char vChar;
	signed char vSChar;
	unsigned char vUChar;
	float vFloat;
	double vDouble;
	int8_t vInt8;
	int16_t vInt16;
	int32_t vInt32;
	int64_t vInt64;
	uint8_t vUInt8;
	uint16_t vUInt16;
	uint32_t vUInt32;
	uint64_t vUInt64;
};

struct TestValue {
	TestValueRaw value;
	TestValueType type;
};

static const TestValue testValues[TEST_VALUECOUNT] = {
	{ { .vBool = false }, Bool },
	{ { .vBool = true }, Bool },
	{ { .vByte = 0xAB }, Byte },
	{ { .vChar = 127 }, Char },
	{ { .vChar = -128 }, Char },
	{ { .vSChar = 127 }, SChar },
	{ { .vSChar = -128 }, SChar },
	{ { .vUChar = 127 }, UChar },
	{ { .vUChar = 128 }, UChar },
	{ { .vFloat = 1.618 }, Float },
	{ { .vFloat = -3.14 }, Float },
	{ { .vDouble = 2015.0408 }, Double },
	{ { .vDouble = -1009.2010 }, Double },
	{ { .vInt8 = 127 }, Int8 },
	{ { .vInt8 = -128 }, Int8 },
	{ { .vInt16 = 258 }, Int16 },
	{ { .vInt16 = -32382 }, Int16 },
	{ { .vInt32 = 16909060 }, Int32 },
	{ { .vInt32 = -2122153084 }, Int32 },
	{ { .vInt64 = 72623859790382856 }, Int64 },
	{ { .vInt64 = -9114578090645354616 }, Int64 },
	{ { .vUInt8 = 127U }, UInt8 },
	{ { .vUInt8 = 128U }, UInt8 },
	{ { .vUInt16 = 258U }, UInt16 },
	{ { .vUInt16 = 33154U }, UInt16 },
	{ { .vUInt32 = 16909060U }, UInt32 },
	{ { .vUInt32 = 2172814212U }, UInt32 },
	{ { .vUInt64 = 72623859790382856U }, UInt64 },
	{ { .vUInt64 = 9332165983064197000U }, UInt64 },
};

static const char littleEndianBytes[TEST_BYTECOUNT] = {
	(char)0x00,
	(char)0x01,
	(char)0xAB,
	(char)0x7F,
	(char)0x80,
	(char)0x7F,
	(char)0x80,
	(char)0x7F,
	(char)0x80,
	(char)0xA0, (char)0x1A, (char)0xCF, (char)0x3F,
	(char)0xC3, (char)0xF5, (char)0x48, (char)0xC0,
	(char)0xB5, (char)0xA6, (char)0x79, (char)0xC7, (char)0x29, (char)0x7C, (char)0x9F, (char)0x40,
	(char)0xF8, (char)0x53, (char)0xE3, (char)0xA5, (char)0x9B, (char)0x89, (char)0x8F, (char)0xC0,
	(char)0x7F,
	(char)0x80,
	(char)0x02, (char)0x01,
	(char)0x82, (char)0x81,
	(char)0x04, (char)0x03, (char)0x02, (char)0x01,
	(char)0x84, (char)0x83, (char)0x82, (char)0x81,
	(char)0x08, (char)0x07, (char)0x06, (char)0x05, (char)0x04, (char)0x03, (char)0x02, (char)0x01,
	(char)0x88, (char)0x87, (char)0x86, (char)0x85, (char)0x84, (char)0x83, (char)0x82, (char)0x81,
	(char)0x7F,
	(char)0x80,
	(char)0x02, (char)0x01,
	(char)0x82, (char)0x81,
	(char)0x04, (char)0x03, (char)0x02, (char)0x01,
	(char)0x84, (char)0x83, (char)0x82, (char)0x81,
	(char)0x08, (char)0x07, (char)0x06, (char)0x05, (char)0x04, (char)0x03, (char)0x02, (char)0x01,
	(char)0x88, (char)0x87, (char)0x86, (char)0x85, (char)0x84, (char)0x83, (char)0x82, (char)0x81,
};

static const char bigEndianBytes[TEST_BYTECOUNT] = {
	(char)0x00,
	(char)0x01,
	(char)0xAB,
	(char)0x7F,
	(char)0x80,
	(char)0x7F,
	(char)0x80,
	(char)0x7F,
	(char)0x80,
	(char)0x3F, (char)0xCF, (char)0x1A, (char)0xA0,
	(char)0xC0, (char)0x48, (char)0xF5, (char)0xC3,
	(char)0x40, (char)0x9F, (char)0x7C, (char)0x29, (char)0xC7, (char)0x79, (char)0xA6, (char)0xB5, 
	(char)0xC0, (char)0x8F, (char)0x89, (char)0x9B, (char)0xA5, (char)0xE3, (char)0x53, (char)0xF8,
	(char)0x7F,
	(char)0x80,
	(char)0x01, (char)0x02,
	(char)0x81, (char)0x82,
	(char)0x01, (char)0x02, (char)0x03, (char)0x04,
	(char)0x81, (char)0x82, (char)0x83, (char)0x84,
	(char)0x01, (char)0x02, (char)0x03, (char)0x04, (char)0x05, (char)0x06, (char)0x07, (char)0x08,
	(char)0x81, (char)0x82, (char)0x83, (char)0x84, (char)0x85, (char)0x86, (char)0x87, (char)0x88,
	(char)0x7F,
	(char)0x80,
	(char)0x01, (char)0x02, 
	(char)0x81, (char)0x82, 
	(char)0x01, (char)0x02, (char)0x03, (char)0x04, 
	(char)0x81, (char)0x82, (char)0x83, (char)0x84, 
	(char)0x01, (char)0x02, (char)0x03, (char)0x04, (char)0x05, (char)0x06, (char)0x07, (char)0x08, 
	(char)0x81, (char)0x82, (char)0x83, (char)0x84, (char)0x85, (char)0x86, (char)0x87, (char)0x88, 
};

static const vector<byte> littleEndianVectors[TEST_VALUECOUNT] = {
	{ (byte)0x00, },
	{ (byte)0x01, },
	{ (byte)0xAB, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0xA0, (byte)0x1A, (byte)0xCF, (byte)0x3F, },
	{ (byte)0xC3, (byte)0xF5, (byte)0x48, (byte)0xC0, },
	{ (byte)0xB5, (byte)0xA6, (byte)0x79, (byte)0xC7, (byte)0x29, (byte)0x7C, (byte)0x9F, (byte)0x40, },
	{ (byte)0xF8, (byte)0x53, (byte)0xE3, (byte)0xA5, (byte)0x9B, (byte)0x89, (byte)0x8F, (byte)0xC0, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x02, (byte)0x01, },
	{ (byte)0x82, (byte)0x81, },
	{ (byte)0x04, (byte)0x03, (byte)0x02, (byte)0x01, },
	{ (byte)0x84, (byte)0x83, (byte)0x82, (byte)0x81, },
	{ (byte)0x08, (byte)0x07, (byte)0x06, (byte)0x05, (byte)0x04, (byte)0x03, (byte)0x02, (byte)0x01, },
	{ (byte)0x88, (byte)0x87, (byte)0x86, (byte)0x85, (byte)0x84, (byte)0x83, (byte)0x82, (byte)0x81, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x02, (byte)0x01, },
	{ (byte)0x82, (byte)0x81, },
	{ (byte)0x04, (byte)0x03, (byte)0x02, (byte)0x01, },
	{ (byte)0x84, (byte)0x83, (byte)0x82, (byte)0x81, },
	{ (byte)0x08, (byte)0x07, (byte)0x06, (byte)0x05, (byte)0x04, (byte)0x03, (byte)0x02, (byte)0x01, },
	{ (byte)0x88, (byte)0x87, (byte)0x86, (byte)0x85, (byte)0x84, (byte)0x83, (byte)0x82, (byte)0x81, },
};

static const vector<byte> bigEndianVectors[TEST_VALUECOUNT] = {
	{ (byte)0x00, },
	{ (byte)0x01, },
	{ (byte)0xAB, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x3F, (byte)0xCF, (byte)0x1A, (byte)0xA0, },
	{ (byte)0xC0, (byte)0x48, (byte)0xF5, (byte)0xC3, },
	{ (byte)0x40, (byte)0x9F, (byte)0x7C, (byte)0x29, (byte)0xC7, (byte)0x79, (byte)0xA6, (byte)0xB5,  },
	{ (byte)0xC0, (byte)0x8F, (byte)0x89, (byte)0x9B, (byte)0xA5, (byte)0xE3, (byte)0x53, (byte)0xF8, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x01, (byte)0x02, },
	{ (byte)0x81, (byte)0x82, },
	{ (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, },
	{ (byte)0x81, (byte)0x82, (byte)0x83, (byte)0x84, },
	{ (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x05, (byte)0x06, (byte)0x07, (byte)0x08, },
	{ (byte)0x81, (byte)0x82, (byte)0x83, (byte)0x84, (byte)0x85, (byte)0x86, (byte)0x87, (byte)0x88, },
	{ (byte)0x7F, },
	{ (byte)0x80, },
	{ (byte)0x01, (byte)0x02,  },
	{ (byte)0x81, (byte)0x82,  },
	{ (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, },
	{ (byte)0x81, (byte)0x82, (byte)0x83, (byte)0x84, },
	{ (byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x05, (byte)0x06, (byte)0x07, (byte)0x08,  },
	{ (byte)0x81, (byte)0x82, (byte)0x83, (byte)0x84, (byte)0x85, (byte)0x86, (byte)0x87, (byte)0x88,  },
};

// helpers
string bytesToString(const vector<byte>& bytes);
bool compareFiles(const char* testFile, const char* staticFile);
void removeTestFiles();
void writeTestStaticFiles();

// tests
bool testBitConverterLittleEndian();
bool testBitConverterBigEndian();
bool testBitConverter(Endian endian, const vector<byte>* staticBytes);
bool testReadLittleEndian();
bool testReadBigEndian();
bool testRead(BinaryReader& br);
bool testWriteLittleEndian();
bool testWriteBigEndian();
bool testWrite(BinaryWriter& bw);

int main(int argc, char** argv) {
	bool allTestsPassed = true;
	Logger::Instance()->openLogFile(BINARYIO_LOG, true);
	
	removeTestFiles();
	writeTestStaticFiles();

	LOG_INFO("Testing bit converter (little endian)");
	bool ret = testBitConverterLittleEndian();
	LOG_INFO("BitConverterLittleEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	LOG_INFO("Testing bit converter (big endian)");
	ret = testBitConverterBigEndian();
	LOG_INFO("BitConverterBigEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	LOG_INFO("Testing read (little endian)");
	ret = testReadLittleEndian();
	LOG_INFO("ReadLittleEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	LOG_INFO("Testing read (big endian)");
	ret = testReadBigEndian();
	LOG_INFO("ReadBigEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	LOG_INFO("Testing write (little endian)");
	ret = testWriteLittleEndian();
	LOG_INFO("WriteLittleEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	LOG_INFO("Testing write (big endian)");
	ret = testWriteBigEndian();
	LOG_INFO("WriteBigEndian test: %s", ret ? "PASS" : "FAIL");
	allTestsPassed = ret ? allTestsPassed : false;

	return (allTestsPassed ? 0 : 1);
}

string bytesToString(const vector<byte>& bytes) {
	string str = "";
	if (bytes.size() == 0) {
		return str;
	}

	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex;
	for (int i = 0; i < bytes.size(); i++) {
		ss << bytes[i];
	}
	str = ss.str();

	return str;
}

bool compareFiles(const char* testFile, const char* staticFile) {
	ifstream tFile = ifstream(testFile, ios::ate | ios::binary);
	ifstream sFile = ifstream(staticFile, ios::ate | ios::binary);

	if (tFile.tellg() != sFile.tellg()) {
		LOG_INFO("File sizes do not match; testFile size = %i, staticFile size = %i", (int)tFile.tellg(), (int)sFile.tellg());
		return false;
	}

	tFile.seekg(0);
	sFile.seekg(0);

	std::istreambuf_iterator<char> beginTFile(tFile);
	std::istreambuf_iterator<char> beginSFile(sFile);

	return std::equal(beginTFile, std::istreambuf_iterator<char>(), beginSFile);
}

void removeTestFiles() {
	remove(TEST_WRITELE);
	remove(TEST_STATICLE);
	remove(TEST_WRITEBE);
	remove(TEST_STATICBE);
}

void writeTestStaticFiles() {
	ofstream stream;

	stream.open((TEST_STATICLE), ios::binary | ios::trunc);
	stream.write(littleEndianBytes, TEST_BYTECOUNT);
	stream.close();

	stream.open((TEST_STATICBE), ios::binary | ios::trunc);
	stream.write(bigEndianBytes, TEST_BYTECOUNT);
	stream.close();
}

bool testBitConverterLittleEndian() {
	return testBitConverter(Little, littleEndianVectors);
/*
	BitConverter::forceSetEndian(endian);

	for (int i = 0; i < TEST_VALUECOUNT; i++) {
		if (!testBitConverter(littleEndianVectors, testValues[i], i)) {
			return false;
		}
	}

	return true;
*/
}

bool testBitConverterBigEndian() {
	return testBitConverter(Big, bigEndianVectors);
/*
	BitConverter::forceSetEndian(endian);

	for (int i = 0; i < TEST_VALUECOUNT; i++) {
		if (!testBitConverter(bigEndianVectors, testValues[i], i)) {
			return false;
		}
	}

	return true;
*/
}

bool testBitConverter(Endian endian, const vector<byte>* staticBytes) {
	BitConverter::forceSetEndian(endian);

	for (int i = 0; i < TEST_VALUECOUNT; i++) {
		switch (testValues[i].type) {
			case Bool: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vBool);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				bool val = BitConverter::getBool(staticBytes[i]);
				if (val != testValues[i].value.vBool) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, testValues[i].value.vBool ? "true" : "false", val ? "true" : "false");
					return false;
				}
				break;
			}
			case Byte:
				continue;
			case Char: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vChar);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				char val = BitConverter::getChar(staticBytes[i]);
				if (val != testValues[i].value.vChar) {
					LOG_INFO("getChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vChar, val);
					return false;
				}
				break;
			}
			case SChar: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vSChar);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				signed char val = BitConverter::getSChar(staticBytes[i]);
				if (val != testValues[i].value.vSChar) {
					LOG_INFO("getSChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vSChar, val);
					return false;
				}
				break;
			}
			case UChar: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vUChar);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				unsigned char val = BitConverter::getUChar(staticBytes[i]);
				if (val != testValues[i].value.vUChar) {
					LOG_INFO("getUChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vUChar, val);
					return false;
				}
				break;
			}
			case Float: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vFloat);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				float val = BitConverter::getFloat(staticBytes[i]);
				if (val != testValues[i].value.vFloat) {
					LOG_INFO("getFloat value incorrect for testValues[%i]; expected: %f, actual: %f", i, testValues[i].value.vFloat, val);
					return false;
				}
				break;
			}
			case Double: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vDouble);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				double val = BitConverter::getDouble(staticBytes[i]);
				if (val != testValues[i].value.vDouble) {
					LOG_INFO("getDouble value incorrect for testValues[%i]; expected: %f, actual: %f", i, testValues[i].value.vDouble, val);
					return false;
				}
				break;
			}
			case Int8: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vInt8);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				int8_t val = BitConverter::getInt8(staticBytes[i]);
				if (val != testValues[i].value.vInt8) {
					LOG_INFO("getInt8 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt8, val);
					return false;
				}
				break;
			}
			case Int16: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vInt16);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				int16_t val = BitConverter::getInt16(staticBytes[i]);
				if (val != testValues[i].value.vInt16) {
					LOG_INFO("getInt16 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt16, val);
					return false;
				}
				break;
			}
			case Int32: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vInt32);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				int32_t val = BitConverter::getInt32(staticBytes[i]);
				if (val != testValues[i].value.vInt32) {
					LOG_INFO("getInt32 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt32, val);
					return false;
				}
				break;
			}
			case Int64: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vInt64);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				int64_t val = BitConverter::getInt64(staticBytes[i]);
				if (val != testValues[i].value.vInt64) {
					LOG_INFO("getInt64 value incorrect for testValues[%i]; expected: %li, actual: %li", i, testValues[i].value.vInt64, val);
					return false;
				}
				break;
			}
			case UInt8: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vUInt8);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				uint8_t val = BitConverter::getUInt8(staticBytes[i]);
				if (val != testValues[i].value.vUInt8) {
					LOG_INFO("getUInt8 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt8, val);
					return false;
				}
				break;
			}
			case UInt16: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vUInt16);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				uint16_t val = BitConverter::getUInt16(staticBytes[i]);
				if (val != testValues[i].value.vUInt16) {
					LOG_INFO("getUInt16 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt16, val);
					return false;
				}
				break;
			}
			case UInt32: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vUInt32);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				uint32_t val = BitConverter::getUInt32(staticBytes[i]);
				if (val != testValues[i].value.vUInt32) {
					LOG_INFO("getUInt32 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt32, val);
					return false;
				}
				break;
			}
			case UInt64: {
				vector<byte> bytes = BitConverter::getBytes(testValues[i].value.vUInt64);
				if (bytes != staticBytes[i]) {
					LOG_INFO("getBytes value incorrect for testValues[%i]; expected: %s, actual: %s", i, bytesToString(staticBytes[i]).c_str(), bytesToString(bytes).c_str());
					return false;
				}
				uint64_t val = BitConverter::getUInt64(staticBytes[i]);
				if (val != testValues[i].value.vUInt64) {
					LOG_INFO("getUInt64 value incorrect for testValues[%i]; expected: %lu, actual: %lu", i, testValues[i].value.vUInt64, val);
					return false;
				}
				break;
			}
			default:
				LOG_INFO("testValues[%d].type is invalid", i);
				return false;
		}
	}

	return true;
}

bool testReadLittleEndian() {
	BinaryReader br(TEST_STATICLE);
	br.forceSetEndian(Little);
	return testRead(br);
}

bool testReadBigEndian() {
	BinaryReader br(TEST_STATICBE);
	br.forceSetEndian(Big);
	return testRead(br);
}

bool testRead(BinaryReader& br) {
	for (int i = 0; i < TEST_VALUECOUNT; i++) {
		switch (testValues[i].type) {
			case Bool: {
				bool val = br.readBool();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vBool != val) {
					LOG_INFO("readBool value incorrect for testValues[%i]; expected: %s, actual: %s", i, testValues[i].value.vBool ? "true" : "false", val ? "true" : "false");
					return false;
				}
				break;
			}
			case Byte: {
				byte val = br.readByte();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vByte != val) {
					LOG_INFO("readByte value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vByte, val);
					return false;
				}
				break;
			}
			case Char: {
				char val = br.readChar();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vChar != val) {
					LOG_INFO("readChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vChar, val);
					return false;
				}
				break;
			}
			case SChar: {
				signed char val = br.readSChar();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vSChar != val) {
					LOG_INFO("readSChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vSChar, val);
					return false;
				}
				break;
			}
			case UChar: {
				unsigned char val = br.readUChar();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vUChar != val) {
					LOG_INFO("readUChar value incorrect for testValues[%i]; expected: 0x%2X, actual: 0x%2X", i, testValues[i].value.vUChar, val);
					return false;
				}
				break;
			}
			case Float: {
				float val = br.readFloat();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vFloat != val) {
					LOG_INFO("readFloat value incorrect for testValues[%i]; expected: %f, actual: %f", i, testValues[i].value.vFloat, val);
					return false;
				}
				break;
			}
			case Double: {
				double val = br.readDouble();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vDouble != val) {
					LOG_INFO("readDouble value incorrect for testValues[%i]; expected: %f, actual: %f", i, testValues[i].value.vDouble, val);
					return false;
				}
				break;
			}
			case Int8: {
				int8_t val = br.readInt8();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vInt8 != val) {
					LOG_INFO("readInt8 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt8, val);
					return false;
				}
				break;
			}
			case Int16: {
				int16_t val = br.readInt16();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vInt16 != val) {
					LOG_INFO("readInt16 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt16, val);
					return false;
				}
				break;
			}
			case Int32: {
				int32_t val = br.readInt32();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vInt32 != val) {
					LOG_INFO("readInt32 value incorrect for testValues[%i]; expected: %i, actual: %i", i, testValues[i].value.vInt32, val);
					return false;
				}
				break;
			}
			case Int64: {
				int64_t val = br.readInt64();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vInt64 != val) {
					LOG_INFO("readInt32 value incorrect for testValues[%i]; expected: %li, actual: %li", i, testValues[i].value.vInt64, val);
					return false;
				}
				break;
			}
			case UInt8: {
				uint8_t val = br.readUInt8();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vUInt8 != val) {
					LOG_INFO("readUInt8 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt8, val);
					return false;
				}
				break;
			}
			case UInt16: {
				uint16_t val = br.readUInt16();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vUInt16 != val) {
					LOG_INFO("readUInt16 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt16, val);
					return false;
				}
				break;
			}
			case UInt32: {
				uint32_t val = br.readUInt32();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vUInt32 != val) {
					LOG_INFO("readUInt32 value incorrect for testValues[%i]; expected: %u, actual: %u", i, testValues[i].value.vUInt32, val);
					return false;
				}
				break;
			}
			case UInt64: {
				uint64_t val = br.readUInt64();
				if (br.hasError()) {
					LOG_INFO("Read error");
					return false;
				}
				if (testValues[i].value.vUInt64 != val) {
					LOG_INFO("readUInt64 value incorrect for testValues[%i]; expected: %lu, actual: %lu", i, testValues[i].value.vUInt64, val);
					return false;
				}
				break;
			}
			default:
				LOG_INFO("testValues[%d].type is invalid", i);
				return false;
		}
	}

	return true;
}

bool testWriteLittleEndian() {
	bool testRet;
	{
		BinaryWriter bw(TEST_WRITELE);
		bw.forceSetEndian(Little);
		testRet = testWrite(bw);
	}
	if (testRet) {
		return compareFiles(TEST_WRITELE, TEST_STATICLE);
	} else {
		return false;
	}
}

bool testWriteBigEndian() {
	bool testRet;
	{
		BinaryWriter bw(TEST_WRITEBE);
		bw.forceSetEndian(Big);
		testRet = testWrite(bw);
	}
	if (testRet) {
		return compareFiles(TEST_WRITEBE, TEST_STATICBE);
	} else {
		return false;
	}
}

bool testWrite(BinaryWriter& bw) {
	for (int i = 0; i < TEST_VALUECOUNT; i++) {
		switch (testValues[i].type) {
			case Bool:
				bw.write(testValues[i].value.vBool);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Byte:
				bw.write(testValues[i].value.vByte);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Char:
				bw.write(testValues[i].value.vChar);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case SChar:
				bw.write(testValues[i].value.vSChar);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case UChar:
				bw.write(testValues[i].value.vUChar);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Float:
				bw.write(testValues[i].value.vFloat);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Double:
				bw.write(testValues[i].value.vDouble);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Int8:
				bw.write(testValues[i].value.vInt8);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Int16:
				bw.write(testValues[i].value.vInt16);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Int32:
				bw.write(testValues[i].value.vInt32);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case Int64:
				bw.write(testValues[i].value.vInt64);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case UInt8:
				bw.write(testValues[i].value.vUInt8);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case UInt16:
				bw.write(testValues[i].value.vUInt16);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case UInt32:
				bw.write(testValues[i].value.vUInt32);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			case UInt64:
				bw.write(testValues[i].value.vUInt64);
				if (bw.hasError()) {
					LOG_INFO("Write error");
					return false;
				}
				break;
			default:
				LOG_INFO("testValues[%d].type is invalid", i);
				return false;
		}
	}

	return true;
}
