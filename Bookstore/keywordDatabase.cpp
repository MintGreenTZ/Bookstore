//
// Created by mintgreen on 18-12-14.
//

#include <string>
#include <iostream>
#include "keywordDatabase.h"

extern std::fstream file;

keywordDatabase::keywordDatabase (std::string keyName) {
    file.open(keyName, std::ios::in);
    if (file) {
		file.seekg(0);
        file.read(reinterpret_cast<char *> (&blockCnt), sizeof(int));
		file.read(reinterpret_cast<char *> (&Bin), sizeof(bin));
		file.read(reinterpret_cast<char *> (&List), sizeof(list));
    }else {
        file.open(keyName, std::fstream::out | std::fstream::binary);
		blockCnt = 1;
		Bin.get();
		file.seekp(0);
		List[0].getPre() = List[0].getNext() = -1;
		Block[0] = slice("", "");
        file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
		file.write(reinterpret_cast<const char *> (&Bin), sizeof(bin));
		file.write(reinterpret_cast<const char *> (&List), sizeof(list));
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block));
    }
}

keywordDatabase::~keywordDatabase() {
	file.seekp(0);
	file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
	file.write(reinterpret_cast<const char *> (&List), sizeof(list));
}

void keywordDatabase::add(std::string key, std::string ISBN) {
	slice data(key, ISBN);
	block Block;
    int p = 0;
    while (List[p].getNext() != -1 && List[p].getFirst() < data) p = List[p].getNext();
	file.seekg(blockBegin + sizeof(block) * p);
	file.read(reinterpret_cast<char *> (&Block), sizeof(block));
	for (int i = Block.getSize() - 1; i >= 0; i--)
		if (Block[i] < data) {
			for (int j = Block.getSize() - 1; j > i; j--)
				Block[j + 1] = Block[j];
			Block[i + 1] = data;
			Block.getSize()++;
			List[p].getSize()++;
			break;
		}
	if (Block.getSize() == blockSize * 2) {
		block extend;
		for (int i = 0; i < blockSize; i++)
			extend[i] = Block[blockSize + i];
		List[p].getSize() = List[q].getSize() = Block.getSize() = extend.getSize() = blockSize;
		int q = Bin.get();
		List[q].getNext() = List[p].getNext();
		List[p].getNext() = q;
		List[q].getFirst() = extend[0];
		blockCnt++;
		file.seekp(blockBegin + sizeof(block) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block));
		file.seekp(blockBegin + sizeof(block) * q);
		file.write(reinterpret_cast<const char *> (&extend), sizeof(block));
	}
	else
	{
		file.seekp(blockBegin + sizeof(block) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block));
	}
}

void keywordDatabase::del(std::string key, std::string ISBN) {
	slice data(key, ISBN);
	block Block;
	int p = 0;
	while (List[p].getNext() != -1 && List[p].getFirst() <= data) p = List[p].getNext();
	file.seekg(blockBegin + sizeof(block) * p);
	file.read(reinterpret_cast<char *> (&Block), sizeof(block));
	for (int i = Block.getSize() - 1; i >= 0; i--)
		if (Block[i] == data) {
			for (int j = i; j < Block.getSize() - 1; j++)
				Block[j] = Block[j + 1];
			Block.getSize()--;
			break;
		}
	if (List[p].getPre() != -1 && List[List[p].getPre()].getSize() + List[p].getSize() <= blockSize) {
		int q = List[p].getPre();
		block Pre;
		file.seekg(blockBegin + sizeof(block) * q);
		file.read(reinterpret_cast<char *> (&Pre), sizeof(block));
		for (int i = 0; i < List[p].getSize(); i++)
			Pre[List[q].getSize() + i] = Block[i];
		Pre.getSize += Block.getSize();
		List[List[q].getPre() = List[p].getPre()].getNext() = q;
		Bin.add(p);
		file.seekp(blockBegin + sizeof(block) * q);
		file.write(reinterpret_cast<const char *> (&Pre), sizeof(block));
	}
	else if (List[p].getNext() != -1 && List[List[p].getNext()].getSize + List[p].getSize() <= blockSize) {
		int q = List[p].getNext();
		block Next;
		file.seekg(blockBegin + sizeof(block) * q);
		file.read(reinterpret_cast<char *> (&Next), sizeof(block));
		for (int i = 0; i < List[q].getSize(); i++)
			Block[List[p].getSize() + i] = Next[i];
		Block.getSize() += Next.getSize();
		List[List[p].getNext() = List[q].getNext()].getPre() = p;
		Bin.add(q);
		file.seekp(blockBegin + sizeof(block) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block));
	}
	else {
		file.seekp(blockBegin + sizeof(block) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block));
	}
}