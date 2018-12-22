//
// Created by mintgreen on 18-12-17.
//

#ifndef BOOKSTORE_DATABASE_H
#define BOOKSTORE_DATABASE_H

#include <string>
#include <vector>
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
	database(std::string keyName) {
		file.open(keyName, std::fstream::in | std::fstream::out | std::fstream::binary);
		if (file) {
			file.seekg(0);
			file.read(reinterpret_cast<char *> (&blockCnt), sizeof(int));
			file.read(reinterpret_cast<char *> (&List), sizeof(list<T>));
			file.read(reinterpret_cast<char *> (&Bin), sizeof(bin));
		}
		else {
			std::ofstream ost(keyName);
			ost.close();
			file.open(keyName, std::fstream::in | std::fstream::out | std::fstream::binary);
			blockCnt = 1;
			Bin.get(); //the first one we get must be 0
			List[0].getPre() = List[0].getNext() = -1;
			List[0].getSize() = Block.getSize() = 1;
			Block[0] = T();
			file.seekp(0);
			file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
			file.write(reinterpret_cast<const char *> (&List), sizeof(list<T>));
			file.write(reinterpret_cast<const char *> (&Bin), sizeof(bin));
			for (int i = 0; i < listLength; i++)
				file.write(reinterpret_cast<const char *> (&Block), sizeof(block<T>));
		}
	}

	~database() {
		file.seekp(0);
		file.write(reinterpret_cast<const char *> (&blockCnt), sizeof(int));
		file.write(reinterpret_cast<const char *> (&List), sizeof(list<T>));
		file.write(reinterpret_cast<const char *> (&Bin), sizeof(bin));
		file.close();
	}

	//add one element to the database (for all kinds) (need to reload < and <=) (throw error when already have one)
	void add(T data) {
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<T>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		for (int i = Block.getSize() - 1; i >= 0; i--) {
			if (Block[i] == data) error("Invalid");
			if (Block[i] < data) {
				for (int j = Block.getSize() - 1; j > i; j--)
					Block[j + 1] = Block[j];
				Block[i + 1] = data;
				Block.getSize()++;
				List[p].getSize()++;
				List[p].getFirst() = Block[0];
				break;
			}
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

	//delete one element to the database (for all kinds) (need to reload < and <=) (throw error when can't find)
	void del(T data) {
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst() <= data) p = List[p].getNext();
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
		if (!suc_del) error("Invalid");
		if (List[p].getPre() != -1 && List[List[p].getPre()].getSize() + List[p].getSize() <= blockSize) {
			int q = List[p].getPre();
			block<T> Pre;
			file.seekg(blockBegin + sizeof(block<T>) * q);
			file.read(reinterpret_cast<char *> (&Pre), sizeof(block<T>));
			for (int i = 0; i < List[p].getSize(); i++)
				Pre[List[q].getSize() + i] = Block[i];
			Pre.getSize() += Block.getSize();
			List[List[q].getNext() = List[p].getNext()].getPre() = q;;
			Bin.add(p);
			file.seekp(blockBegin + sizeof(block<T>) * q);
			file.write(reinterpret_cast<const char *> (&Pre), sizeof(block<T>));
		}
		else if (List[p].getNext() != -1 && List[List[p].getNext()].getSize() + List[p].getSize() <= blockSize) {
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

	//modify a user's information (throw error when not found)
	void modify(userInfo data) {
		block<userInfo> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<userInfo>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<userInfo>));
		bool suc_modify = 0;
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i] == data) {
				Block[i].update(data);
				suc_modify = 1;
				break;
			}
		if (!suc_modify) error("Invalid");
		file.seekp(blockBegin + sizeof(block<userInfo>) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block<userInfo>));
	}

	//modify books' quantity information (throw error when not found or not sufficient)
	void trade(record data, int det) {
		block<record> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst() <= data) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<record>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<record>));
		bool suc_trade = 0;
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i] == data) {
				if (Block[i].getQuantity() + det < 0) error("Invalid");
				Block[i].getQuantity() += det;
				suc_trade = 1;
				break;
			}
		if (!suc_trade) error("trade fail!");
		file.seekp(blockBegin + sizeof(block<record>) * p);
		file.write(reinterpret_cast<const char *> (&Block), sizeof(block<record>));
	}

	//ask the detailed information with only a key (throw error when not find)
	T ask(standString key) {
		if (!(isSameType(T(), userInfo()) || isSameType(T(), record()))) error("type error at ask!");
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst().getKey() <= key) p = List[p].getNext();
		file.seekg(blockBegin + sizeof(block<T>) * p);
		file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
		for (int i = Block.getSize() - 1; i >= 0; i--)
			if (Block[i].getKey() == key)
				return Block[i];
		error("not find!");
		return T();
	}

	//return all elements' ISBNs which keys are satisfied (catar to both record and slice)
	template <class T>
	std::vector<standString> giveMeAll(standString key) {
		std::vector<standString> ret;
		block<T> Block;
		int p = 0;
		while (List[p].getNext() != -1 && List[List[p].getNext()].getFirst().getKey() < key)
			p = List[p].getNext();
		for (; p != -1 && List[p].getFirst().getKey() <= key; p = List[p].getNext()) {
			file.seekg(blockBegin + sizeof(block<T>) * p);
			file.read(reinterpret_cast<char *> (&Block), sizeof(block<T>));
			for (int i = 0; i < Block.getSize(); i++)
				if (Block[i].getKey() == key)
					ret.push_back(Block[i].getISBN());
		}
		return ret;
	}

	//show all the elements in database (only support record)
	void showAll() {
		block<record> Block;
		for (int p = 0; p != -1; p = List[p].getNext()) {
			file.seekg(blockBegin + sizeof(block<record>) * p);
			file.read(reinterpret_cast<char *> (&Block), sizeof(block<record>));
			for (int i = 0; i < Block.getSize(); i++)
				if (!(p == 0 && i == 0))
					Block[i].print();
		}
	}
};
#endif //BOOKSTORE_DATABASE_H
