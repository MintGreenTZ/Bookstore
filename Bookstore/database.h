//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_DATABASE_H
#define BOOKSTORE_DATABASE_H

#include <string>
#include "tools.h"
#include "error.h"

template <class T>
class database {
	const int listBegin = sizeof(int);
	const int binBegin = listBegin + sizeof(list<T>);
	const int blockBegin = binBegin + sizeof(bin);
	int blockCnt;
	list<T> List;
	bin Bin;
	block<T> Block;
	std::fstream file;
public:
	database(std::string key) {
		file.open(keyName, std::ios::in);
		if (file) {
			file.seekg(0);
			file.read(reinterpret_cast<char *> (&blockCnt), sizeof(int));
			file.read(reinterpret_cast<char *> (&Bin), sizeof(bin));
			file.read(reinterpret_cast<char *> (&List), sizeof(list<T>));
		}
		else {
			file.open(keyName, std::fstream::out | std::fstream::binary);
			blockCnt = 1;
			Bin.get();
			file.seekp(0);
			List[0].getPre() = List[0].getNext() = -1;
			Block[0] = T();
			file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
			file.write(reinterpret_cast<const char *> (&Bin), sizeof(bin));
			file.write(reinterpret_cast<const char *> (&List), sizeof(list<T>));
			file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
		}
	}
	~database() {
		file.seekp(0);
		file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
		file.write(reinterpret_cast<const char *> (&List), sizeof(list<T>));
		file.close();
	}
	void add(T data) {
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[p].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i] == data)
				return;
			else if (Block[i] < key) {
				for (int j = Block.getSize() - 1; j > i; j--)
					Block[j + 1] = Block[j];
				Block[i + 1] = data;
				Block.getSize()++;
				List[p].getSize()++;
				break;
			}
		if (Block.getSize() == blockSize * 2) {
			block<T> extend;
			for (int i = 0; i < blockSize; i++)
				extend[i] = Block[blockSize + i];
			int q = Bin.get();
			List[p].getSize() = List[q].getSize() = Block.getSize() = extend.getSize() = blockSize;
			List[q].getNext() = List[p].getNext();
			List[p].getNext() = q;
			List[q].getFirst() = extend[0];
			blockCnt++;
			file.seekp(blockBegin + sizeof(block<T>) * p);
			file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
			file.seekp(blockBegin + sizeof(block<T>) * q);
			file.write(reinterpret_cast<const char *> (&extend), sizeof(block<T>));
		}
		else
		{
			file.seekp(blockBegin + sizeof(block<T>) * p);
			file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
		}
	}

	void del(T data) {
		if (!isSameType(T, slice)) error("type error at del!");
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[p].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<T>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		bool suc_del = 0;
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i] == data) {
				for (int j = i; j < Block.getSize() - 1; j++)
					Block[j] = Block[j + 1];
				Block.getSize()--;
				suc_del = 1;
				break;
			}
		if (!suc_del) error("delete fail!");
		if (List[p].getPre() != -1 && List[List[p].getPre()].getSize() + List[p].getSize() <= blockSize) {
			int q = List[p].getPre();
			block<T> Pre;
			file.seekg(blockBegin + sizeof(block<T>) * q);
			file.read(reinterpret_cast<char *> (&Pre), sizeof(block<T>));
			for (int i = 0; i < List[p].getSize(); i++)
				Pre[List[q].getSize() + i] = Block[i];
			Pre.getSize += Block.getSize();
			List[List[q].getPre() = List[p].getPre()].getNext() = q;
			Bin.add(p);
			file.seekp(blockBegin + sizeof(block<T>) * q);
			file.write(reinterpret_cast<const char *> (&Pre), sizeof(block<T>));
		}
		else if (List[p].getNext() != -1 && List[List[p].getNext()].getSize + List[p].getSize() <= blockSize) {
			int q = List[p].getNext();
			block<T> Next;
			file.seekg(blockBegin + sizeof(block<T>) * q);
			file.read(reinterpret_cast<char *> (&Next), sizeof(block<T>));
			for (int i = 0; i < List[q].getSize(); i++)
				Block[List[p].getSize() + i] = Next[i];
			Block.getSize() += Next.getSize();
			List[List[p].getNext() = List[q].getNext()].getPre() = p;
			Bin.add(q);
			file.seekp(blockBegin + sizeof(block<T>) * p);
			file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
		}
		else {
			file.seekp(blockBegin + sizeof(block<T>) * p);
			file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
		}
	}

	void modify(T data) {
		if (!isSameType(T, record)) error("type error at modify!");
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[p].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<T>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		bool suc_modify = 0;
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i] == data) {
				Block[i] = Block[i].update(data);
				suc_modify = 1;
				break;
			}
		if (!suc_modify) error("modify fail!");
		file.seekg(blockBegin + sizeof(block<T>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		file.seekp(blockBegin + sizeof(block<T>) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
	}
};
#endif //BOOKSTORE_DATABASE_H