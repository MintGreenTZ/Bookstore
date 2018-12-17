//
// Created by mintgreen on 18-12-15.
//

#include <string>
#include <iostream>
#include "mainDatabase.h"
#include "tools.h"

extern std::fstream file;

mainDatabase::mainDatabase() {
	file.open("mainDatabase", std::ios::in);
	if (file) {
		file.seekg(0);
		file.read(reinterpret_cast<char *> (&blockCnt), sizeof(int));
		file.read(reinterpret_cast<char *> (&Bin), sizeof(bin));
		file.read(reinterpret_cast<char *> (&List), sizeof(list<record>));
	}
	else {
		file.open("mainDatabase", std::fstream::out | std::fstream::binary);
		blockCnt = 1;
		Bin.get();
		file.seekp(0);
		List[0].getPre() = List[0].getNext() = -1;
		Block[0] = record(standString(""));
		file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
		file.write(reinterpret_cast<const char *> (&Bin), sizeof(bin));
		file.write(reinterpret_cast<const char *> (&List), sizeof(list<record>));
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block<record>));
	}
}

mainDatabase::~mainDatabase() {
	file.seekp(0);
	file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
	file.write(reinterpret_cast<const char *> (&List), sizeof(list<record>));
}

void mainDatabase::add(std::string ISBN) {
	block<record> Block;
	slice key(ISBN);
	int p = 0;
	while (List[p].getNext() != -1 && List[p].getFirst() <= key) p = List[p].getNext();
	file.seekg(blockBegin + sizeof(block<record>) * p);
	file.read(reinterpret_cast<char *> (&Block), sizeof(detailBlock));
	for (int i = Block.getSize() - 1; i >= 0; i--)
		if (slice(Block[i]) == key)
			return;
		else if (slice(Block[i]) < key) {
			for (int j = Block.getSize() - 1; j > i; j--)
				Block[j + 1] = Block[j];
			Block[i + 1] = record(standString(ISBN));
			Block.getSize()++;
			List[p].getSize()++;
			break;
		}
	if (Block.getSize() == blockSize * 2) {
		detailBlock extend;
		for (int i = 0; i < blockSize; i++)
			extend[i] = Block[blockSize + i];
		int q = Bin.get();
		List[p].getSize() = List[q].getSize() = Block.getSize() = extend.getSize() = blockSize;
		List[q].getNext() = List[p].getNext();
		List[p].getNext() = q;
		List[q].getFirst() = key;
		blockCnt++;
		file.seekp(blockBegin + sizeof(detailBlock) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(detailBlock));
		file.seekp(blockBegin + sizeof(detailBlock) * q);
		file.write(reinterpret_cast<const char *> (&extend), sizeof(detailBlock));
	}
	else
	{
		file.seekp(blockBegin + sizeof(detailBlock) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(detailBlock));
	}
}

void mainDatabase::modify(record data) {
	detailBlock Block;
	slice key(data.getISBN());
	int p = 0;
	while (List[p].getNext() != -1 && List[p].getFirst() <= key) p = List[p].getNext();
	file.seekg(blockBegin + sizeof(detailBlock) * p);
	file.read(reinterpret_cast<char *> (&Block), sizeof(detailBlock));
	for (int i = Block.getSize() - 1; i >= 0; i--)
		if (slice(Block[i]) == key) {
			Block[i] = Block[i].update(data);
			break;
		}
	file.seekg(blockBegin + sizeof(detailBlock) * p);
	file.read(reinterpret_cast<char *> (&Block), sizeof(detailBlock));
	file.seekp(blockBegin + sizeof(detailBlock) * p);
	file.write(reinterpret_cast<const char *> (&Block), sizeof(detailBlock));
}