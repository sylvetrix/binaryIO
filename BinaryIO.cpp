#include <cstring>
#include <stdexcept>

#include "BinaryIO.h"

bool BitConverter::forceEndian = false;
Endian BitConverter::endianOverride = endian;

bool BitConverter::isLittleEndian() {
	return (!forceEndian ? endian : endianOverride);
}

void BitConverter::forceSetEndian(Endian newEndian) {
	forceEndian = true;
	endianOverride = newEndian;
}

void BitConverter::forceUnsetEndian() {
	forceEndian = false;
	endianOverride = endian;
}

vector<byte> BitConverter::getBytes(bool value) {
	return getBytes1(value ? (uint8_t)1 : (uint8_t)0);
}

vector<byte> BitConverter::getBytes(char value) {
	return getBytes1((uint8_t)value);
}

vector<byte> BitConverter::getBytes(signed char value) {
	return getBytes1((uint8_t)value);
}

vector<byte> BitConverter::getBytes(unsigned char value) {
	return getBytes1((uint8_t)value);
}

vector<byte> BitConverter::getBytes(float value) {
	uint32_t valueBytes;
	memcpy(&valueBytes, &value, 4);
	return getBytes4(valueBytes);
}

vector<byte> BitConverter::getBytes(double value) {
	uint64_t valueBytes;
	memcpy(&valueBytes, &value, 8);
	return getBytes8(valueBytes);
}

/*
vector<byte> BitConverter::getBytes(int8_t value) {
	return getBytes1((uint8_t)value);
}
*/

vector<byte> BitConverter::getBytes(int16_t value) {
	return getBytes2((uint16_t)value);
}

vector<byte> BitConverter::getBytes(int32_t value) {
	return getBytes4((uint32_t)value);
}

vector<byte> BitConverter::getBytes(int64_t value) {
	return getBytes8((uint64_t)value);
}

/*
vector<byte> BitConverter::getBytes(uint8_t value) {
	return getBytes1(value);
}
*/

vector<byte> BitConverter::getBytes(uint16_t value) {
	return getBytes2(value);
}

vector<byte> BitConverter::getBytes(uint32_t value) {
	return getBytes4(value);
}

vector<byte> BitConverter::getBytes(uint64_t value) {
	return getBytes8(value);
}

bool BitConverter::getBool(vector<byte> bytes) {
	return (bool)getValue1(bytes);
}

char BitConverter::getChar(vector<byte> bytes) {
	return (char)getValue1(bytes);
}

signed char BitConverter::getSChar(vector<byte> bytes) {
	return (signed char)getValue1(bytes);
}

unsigned char BitConverter::getUChar(vector<byte> bytes) {
	return (unsigned char)getValue1(bytes);
}

float BitConverter::getFloat(vector<byte> bytes) {
	float value;
	uint32_t valueBytes = getValue4(bytes);
	memcpy(&value, &valueBytes, 4);
	return value;
}

double BitConverter::getDouble(vector<byte> bytes) {
	double value;
	uint64_t valueBytes = getValue8(bytes);
	memcpy(&value, &valueBytes, 8);
	return value;
}

int8_t BitConverter::getInt8(vector<byte> bytes) {
	return (int8_t)getValue1(bytes);
}

int16_t BitConverter::getInt16(vector<byte> bytes) {
	return (int16_t)getValue2(bytes);
}

int32_t BitConverter::getInt32(vector<byte> bytes) {
	return (int32_t)getValue4(bytes);
}

int64_t BitConverter::getInt64(vector<byte> bytes) {
	return (int64_t)getValue8(bytes);
}

uint8_t BitConverter::getUInt8(vector<byte> bytes) {
	return getValue1(bytes);
}

uint16_t BitConverter::getUInt16(vector<byte> bytes) {
	return getValue2(bytes);
}

uint32_t BitConverter::getUInt32(vector<byte> bytes) {
	return getValue4(bytes);
}

uint64_t BitConverter::getUInt64(vector<byte> bytes) {
	return getValue8(bytes);
}

vector<byte> BitConverter::getBytes1(uint8_t value) {
	vector<byte> retval = vector<byte>(1);
	retval[0] = (byte)value;

	return retval;
}

vector<byte> BitConverter::getBytes2(uint16_t value) {
	vector<byte> retval = vector<byte>(2);

	for (int i = 0; i < 2; i++) {
		if (isLittleEndian()) {
			retval[i] = (byte)(value >> (i * 8));
		} else {
			retval[1 - i] = (byte)(value >> (i * 8));
		}
	}

	return retval;
}

vector<byte> BitConverter::getBytes4(uint32_t value) {
	vector<byte> retval = vector<byte>(4);

	for (int i = 0; i < 4; i++) {
		if (isLittleEndian()) {
			retval[i] = (byte)(value >> (i * 8));
		} else {
			retval[3 - i] = (byte)(value >> (i * 8));
		}
	}

	return retval;
}

vector<byte> BitConverter::getBytes8(uint64_t value) {
	vector<byte> retval = vector<byte>(8);

	for (int i = 0; i < 8; i++) {
		if (isLittleEndian()) {
			retval[i] = (byte)(value >> (i * 8));
		} else {
			retval[7 - i] = (byte)(value >> (i * 8));
		}
	}

	return retval;
}

uint8_t BitConverter::getValue1(vector<byte> bytes) {
	if (bytes.size() < 1) {
		throw std::runtime_error("byte vector is too small");
	}

	return ((uint8_t)bytes[0]);
}

uint16_t BitConverter::getValue2(vector<byte> bytes) {
	if (bytes.size() < 2) {
		throw std::runtime_error("byte vector is too small");
	}

	uint16_t retval = 0;
	for (int i = 0; i < 2; i++) {
		if (isLittleEndian()) {
			retval += ((uint16_t)bytes[i] << (i * 8));
		} else {
			retval += ((uint16_t)bytes[1 - i] << (i * 8));
		}
	}

	return retval;
}

uint32_t BitConverter::getValue4(vector<byte> bytes) {
	if (bytes.size() < 4) {
		throw std::runtime_error("byte vector is too small");
	}

	uint32_t retval = 0;
	for (int i = 0; i < 4; i++) {
		if (isLittleEndian()) {
			retval += ((uint32_t)bytes[i] << (i * 8));
		} else {
			retval += ((uint32_t)bytes[3 - i] << (i * 8));
		}
	}

	return retval;
}

uint64_t BitConverter::getValue8(vector<byte> bytes) {
	if (bytes.size() < 8) {
		throw std::runtime_error("byte vector is too small");
	}

	uint64_t retval = 0;
	for (int i = 0; i < 8; i++) {
		if (isLittleEndian()) {
			retval += ((uint64_t)bytes[i] << (i * 8));
		} else {
			retval += ((uint64_t)bytes[7 - i] << (i * 8));
		}
	}

	return retval;
}

BinaryIOBase::BinaryIOBase(string fileLocation, ios::openmode mode) {
	bufferPos = 0;
	bufferDataSize = 0;
	lastError = None;
	forceEndian = false;
	this->fileLocation = fileLocation;
	this->mode = mode;
	stream.open(this->fileLocation, this->mode);
	if (!stream.is_open()) {
		lastError = CannotOpenFile;
	}
}

BinaryIOBase::~BinaryIOBase() {
	if (stream.is_open()) {
		stream.close();
	}
}

bool BinaryIOBase::hasError() {
	return (lastError != None);
}

BinaryIOError BinaryIOBase::getError() {
	return lastError;
}

void BinaryIOBase::forceSetEndian(Endian newEndian) {
	forceEndian = true;
	endianOverride = newEndian;
}

void BinaryIOBase::forceUnsetEndian() {
	forceEndian = false;
	endianOverride = endian;
}

bool BinaryIOBase::isLittleEndian() {
	return (!forceEndian ? endian : endianOverride);
}

BinaryReader::BinaryReader(const char* fileLocation) : BinaryIOBase(string(fileLocation), ios::in | ios::binary) {
	
}

BinaryReader::BinaryReader(string fileLocation) : BinaryIOBase(fileLocation, ios::in | ios::binary) {
	
}

BinaryReader::~BinaryReader() {
	
}

bool BinaryReader::moreData() {
	return (!stream.eof() || (bufferPos != bufferDataSize));
}

bool BinaryReader::readBool() {
	uint8_t value = read1();
	return ((value == 0) ? false : true);
}

byte BinaryReader::readByte() {
	return (byte)read1();
}

char BinaryReader::readChar() {
	return (char)read1();
}

signed char BinaryReader::readSChar() {
	return (signed char)read1();
}

unsigned char BinaryReader::readUChar() {
	return (unsigned char)read1();
}

float BinaryReader::readFloat() {
	float value;
	uint32_t valueBytes = read4();
	memcpy(&value, &valueBytes, 4);
	return value;
}

double BinaryReader::readDouble() {
	double value;
	uint64_t valueBytes = read8();
	memcpy(&value, &valueBytes, 8);
	return value;
}

int8_t BinaryReader::readInt8() {
	return (int8_t)read1();
}

int16_t BinaryReader::readInt16() {
	return (int16_t)read2();
}

int32_t BinaryReader::readInt32() {
	return (int32_t)read4();
}

int64_t BinaryReader::readInt64() {
	return (int64_t)read8();
}

uint8_t BinaryReader::readUInt8() {
	return read1();
}

uint16_t BinaryReader::readUInt16() {
	return read2();
}

uint32_t BinaryReader::readUInt32() {
	return read4();
}

uint64_t BinaryReader::readUInt64() {
	return read8();
}

vector<byte> BinaryReader::readBytes(int count) {
	vector<byte> bytes = vector<byte>(count);

	for (int i = 0; i < count; i++) {
		bytes[i] = readByte();
		if (hasError()) {
			return vector<byte>(0);
		}
	}

	return bytes;
}

uint8_t BinaryReader::read1() {
	uint8_t value = 0;
	if (bufferPos >= bufferDataSize) {
		readNextChunk();
	}
	if (!moreData()) {
		lastError = NotEnoughData;
		return value;
	}

	value = buffer[bufferPos];
	bufferPos++;
	return value;
}

uint16_t BinaryReader::read2() {
	uint16_t value = 0;

	for (int i = 0; i < 16; i += 8) {
		if (hasError()) {
			return 0U;
		}
		if (isLittleEndian()) {
			value += ((uint16_t)read1() << i);
		} else {
			value += ((uint16_t)read1() << (8 * 1 - i));
		}
	}

	return value;
}

uint32_t BinaryReader::read4() {
	uint32_t value = 0;

	for (int i = 0; i < 32; i += 8) {
		if (hasError()) {
			return 0U;
		}
		if (endian == isLittleEndian()) {
			value += ((uint32_t)read1() << i);
		} else {
			value += ((uint32_t)read1() << (8 * 3 - i));
		}
	}

	return value;
}

uint64_t BinaryReader::read8() {
	uint64_t value = 0;

	for (int i = 0; i < 64; i += 8) {
		if (hasError()) {
			return 0U;
		}
		if (isLittleEndian()) {
			value += ((uint64_t)read1() << i);
		} else {
			value += ((uint64_t)read1() << (8 * 7 - i));
		}
	}

	return value;
}

void BinaryReader::readNextChunk() {
	bufferPos = 0;
	bufferDataSize = 0;
	if (stream.is_open()) {
		stream.readsome(buffer, BUFFERMAX);
		if (stream.fail() || stream.bad()) {
			lastError = GenericReadError;
			return;
		}
		bufferDataSize = stream.gcount();
	}
}

BinaryWriter::BinaryWriter(const char* fileLocation, bool overwrite) : BinaryIOBase(string(fileLocation), ios::out | ios::binary | (overwrite ? ios::trunc : ios::app)) {

}

BinaryWriter::BinaryWriter(string fileLocation, bool overwrite) : BinaryIOBase(fileLocation, ios::out | ios::binary | (overwrite ? ios::trunc : ios::app)) {

}

BinaryWriter::~BinaryWriter() {
	flush();
}

void BinaryWriter::write(bool value) {
	write1((uint8_t)value);
}

/*
void BinaryWriter::write(byte value) {
	write1((uint8_t)value);
}
*/

void BinaryWriter::write(char value) {
	write1((uint8_t)value);
}

void BinaryWriter::write(signed char value) {
	write1((uint8_t)value);
}

void BinaryWriter::write(unsigned char value) {
	write1((uint8_t)value);
}

void BinaryWriter::write(float value) {
	uint32_t valueBytes;
	memcpy(&valueBytes, &value, 4);
	write4(valueBytes);
}

void BinaryWriter::write(double value) {
	uint64_t valueBytes;
	memcpy(&valueBytes, &value, 8);
	write8(valueBytes);
}

/*
void BinaryWriter::write(int8_t value) {
	write1((uint8_t)value);
}
*/

void BinaryWriter::write(int16_t value) {
	write2((uint16_t)value);
}

void BinaryWriter::write(int32_t value) {
	write4((uint32_t)value);
}

void BinaryWriter::write(int64_t value) {
	write8((uint64_t)value);
}

/*
void BinaryWriter::write(uint8_t value) {
	write1(value);
}
*/

void BinaryWriter::write(uint16_t value) {
	write2(value);
}

void BinaryWriter::write(uint32_t value) {
	write4(value);
}

void BinaryWriter::write(uint64_t value) {
	write8(value);
}

void BinaryWriter::write(vector<byte> bytes) {
	write(bytes, 0, bytes.size());
}

void BinaryWriter::write(vector<byte> bytes, int start, int count) {
	if ((start >= bytes.size()) || (start + count > bytes.size())) {
		lastError = NotEnoughData;
		return;
	}

	for (int i = start; i < (start + count); i++) {
		if (hasError()) {
			return;
		}
		write1((uint8_t)bytes[i]);
	}
}

void BinaryWriter::write1(uint8_t value) {
	if (bufferPos >= BUFFERMAX) {
		flush();
	}
	if (hasError()) {
		return;
	}

	buffer[bufferPos] = value;
	bufferPos++;
}

void BinaryWriter::write2(uint16_t value) {
	for (int i = 0; i < 16; i += 8) {
		if (hasError()) {
			return;
		}
		if (isLittleEndian()) {
			write1((uint8_t)((value >> i) & 0xFF));
		} else {
			write1((uint8_t)((value >> (8 * 1 - i)) & 0xFF));
		}
	}
}

void BinaryWriter::write4(uint32_t value) {
	for (int i = 0; i < 32; i += 8) {
		if (hasError()) {
			return;
		}
		if (isLittleEndian()) {
			write1((uint8_t)((value >> i) & 0xFF));
		} else {
			write1((uint8_t)((value >> (8 * 3 - i)) & 0xFF));
		}
	}
}

void BinaryWriter::write8(uint64_t value) {
	for (int i = 0; i < 64; i += 8) {
		if (hasError()) {
			return;
		}
		if (isLittleEndian()) {
			write1((uint8_t)((value >> i) & 0xFF));
		} else {
			write1((uint8_t)((value >> (8 * 7 - i)) & 0xFF));
		}
	}
}

void BinaryWriter::flush() {
	if (stream.is_open()) {
		stream.write(buffer, bufferPos);
		bufferPos = 0;
		if (stream.fail() || stream.bad()) {
			lastError = GenericWriteError;
		}
	}
}

