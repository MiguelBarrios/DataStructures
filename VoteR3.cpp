//
//  main.cpp
//  Project2VoteR
//
//  Created by Miguel Barrios on 10/22/18.
//  Copyright © 2018 Miguel Barrios. All rights reserved.
//

#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include "NvraRecord.h"
#include "TemplatedArray.h"
#include "NvraComparator.h"
#include "Sorter.h"
#include "Search.h"
#include "OULink.h"
#include "OULinkedList.h"
#include "NvraHasher.h"
#include "Hasher.h"
#include "HashTable.h"
#include "HashTableEnumerator.h"
#include "AVLTree.h"
#include "AVLTreeEnumerator.h"
#include "AVLTreeOrder.h"


struct filesSummary* startUp();
void merge(struct filesSummary* dataInfo);
void purge(struct filesSummary* dataInfo);
bool readInFile(struct filesSummary* dataInfo, string input);
NvraRecord* readRecord(string recordInfo, int& currentLineCount);
void userOptionLoop(struct filesSummary* nvraArray);
void find(struct filesSummary* nvraArray);
void searchForStringData(filesSummary* dataInfo, int searchCollum);
void searchForNumericData(filesSummary* dataInfo, int searchCollum);
void sort(struct filesSummary* nvraArray);
int isValidSortField(string sortField);
void linearSearch(TemplatedArray<NvraRecord>* array, NvraComparator comparatror, NvraRecord value);
void output(TemplatedArray<NvraRecord>* array, unsigned long validLineCount, unsigned long lineCount);
void displayHashTable(HashTable<NvraRecord>* hashTable, AVLTree<NvraRecord>* tree, unsigned long validLineCount, unsigned long lineCount);
void printTree(filesSummary* dataInfo, AVLTreeOrder order);
string dataSummery(AVLTree<NvraRecord>* tree, unsigned long validLineCount, unsigned long lineCount);
void updateFiles(filesSummary* dataInfo);

using namespace std;


struct filesSummary
{
    AVLTree<NvraRecord>* tree;
    TemplatedArray<NvraRecord>* nvraArray;
    OULinkedList<NvraRecord>* linkedList;
    HashTable<NvraRecord>* hashTable;
    unsigned long totalLineCount;
    unsigned long validLinecount;
    int sortedOn = -1;
};

int main()
{
    struct filesSummary* files = startUp();
    
    if(files -> sortedOn == -1)                                 //No files were read in Exits program
        return 0;
    
    userOptionLoop(files);
    
    return 0;
}

struct filesSummary* startUp()
{
    struct filesSummary* RecordsInfo = new filesSummary();
    if(RecordsInfo == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    RecordsInfo -> tree = new AVLTree<NvraRecord>(new NvraComparator(0));
    if(RecordsInfo == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    Comparator<NvraRecord>* comparator = new NvraComparator(0);
    if(comparator == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    RecordsInfo -> totalLineCount = 1;
    RecordsInfo -> validLinecount = 0;
    merge(RecordsInfo);
    return RecordsInfo;
}

//Used for sorting and outputting Record data
void userOptionLoop(struct filesSummary* dataInfo)
{
    string selection = "";
    cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (h)ash table, (pre)order, (in)order, (post)order, or (q)uit: ";
    getline(cin, selection);
    while(selection != "q")
    {
        if(selection == "o")
        {
            output(dataInfo -> nvraArray, dataInfo -> validLinecount, dataInfo -> totalLineCount);
        }
        else if(selection == "s")
        {
            sort(dataInfo);
        }
        else if(selection == "f")
        {
            find(dataInfo);
        }
        else if(selection == "m")
        {
            merge(dataInfo);
        }
        else if (selection == "p")
        {
            purge(dataInfo);
        }
        else if (selection == "h")
        {
            displayHashTable(dataInfo -> hashTable, dataInfo -> tree, dataInfo -> validLinecount, dataInfo -> totalLineCount);
        }
        else if (selection == "pre")
        {
            printTree(dataInfo, AVLTreeOrder::preorder);
        }
        else if (selection == "in")
        {
            printTree(dataInfo, AVLTreeOrder::inorder);
        }
        else if (selection == "post")
        {
            printTree(dataInfo, AVLTreeOrder::postorder);
        }
        
        cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (h)ash table, (pre)order, (in)order, (post)order, or (q)uit: ";
        getline(cin, selection);
    }
    cout << "Thanks for using VoteR." << endl;
}

void merge(struct filesSummary* dataInfo)
{
    cout << "Enter data file name: ";
    string fileName = "";
    getline(cin, fileName);
    //Loops untill the user enters a valid file name
    while(fileName.length() > 0)
    {
        ifstream myfile(fileName);
        if(myfile.is_open())
        {
            if(readInFile(dataInfo,fileName))
            {
                updateFiles(dataInfo);
                dataInfo -> sortedOn = 0;
                fileName = "";
            }
            else
            {
                cout << "No valid records found.\nEnter data file name: ";
                getline(cin, fileName);
            }
        }
        else
        {
            cout << "File is not available.\nEnter data file name: ";
            getline(cin, fileName);
        }
    }
}

void purge(struct filesSummary* dataInfo)
{
    cout << "Enter data file name: ";
    string fileName = "";
    getline(cin, fileName);
    //Loops untill the user enters a valid file name
    while(fileName.length() > 0)
    {
        ifstream myfile(fileName);
        if(myfile.is_open())
        {
            ifstream myfile(fileName);
            
            int currentLineCount = 1;
            //Discards Header
            string header;
            getline(myfile, header);
            
            string line;
            while(getline(myfile, line))
            { //Parses File line by line
                
                NvraRecord* recordToRemove = readRecord(line, currentLineCount);
                if(recordToRemove != nullptr)
                {
                    if(dataInfo -> tree -> remove(recordToRemove))
                    {
                        dataInfo -> hashTable -> remove(recordToRemove);
                    }
                    ++dataInfo -> validLinecount;

                }
                ++dataInfo -> totalLineCount;
                ++currentLineCount;
            }
            cout << "1 Tree Size: " << dataInfo -> tree -> getSize() << endl;
            fileName = "";
            updateFiles(dataInfo);
        }
        else
        {
            cout << "File is not available.\nEnter data file name: ";
            getline(cin, fileName);
        }
    }
}

//return true if valid record was read in false otherwise
bool readInFile(struct filesSummary* dataInfo, string fileName)
{
    NvraComparator* comparator = new NvraComparator(0);
    if(comparator == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    AVLTree<NvraRecord>* currentRecords = new AVLTree<NvraRecord>(comparator);
    if(currentRecords == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    int currentLineCount = 1;

    ifstream listOfRecords(fileName);
    string header;
    getline(listOfRecords, header);            //Discards Header
    
    string recordData;
    while(getline(listOfRecords, recordData))
    { //Parses File line by line
        
        NvraRecord* newRecord = readRecord(recordData, currentLineCount);
        //checks to see if record was created
        if(newRecord != nullptr)
        {   //Adds if record is not a duplicate within file
            if(!currentRecords -> insert(newRecord))
            {
                //File is a duplicate whithin the current file being read in
                cout << "Duplicate record ID " << newRecord -> getNum(0) << " at line " << to_string(dataInfo -> totalLineCount - 1) << "." << endl;
            }
            ++dataInfo -> validLinecount;
        }
        ++dataInfo -> totalLineCount;
        ++currentLineCount;
    }
    
    if(dataInfo -> tree -> getSize( )== 0)
    {
        dataInfo -> tree = currentRecords;
    }
    else
    {
        AVLTree<NvraRecord>* updatedTree = new AVLTree<NvraRecord>(comparator);
        
        AVLTreeEnumerator<NvraRecord> oldTreeEnumerator(dataInfo -> tree, AVLTreeOrder::inorder);
        AVLTreeEnumerator<NvraRecord> newTreeEnumerator(currentRecords, AVLTreeOrder::inorder);
        
        
        while(oldTreeEnumerator.hasNext())
        {
            NvraRecord record = oldTreeEnumerator.next();
            updatedTree -> insert(&record);
        }
        while(newTreeEnumerator.hasNext())
        {
            NvraRecord record = newTreeEnumerator.next();
            if(!updatedTree -> insert(&record))
            {
                updatedTree -> replace(&record);
            }
        }
        
        delete dataInfo -> tree;
        dataInfo -> tree = nullptr;
        
        dataInfo -> tree = updatedTree;
        
        
        delete currentRecords;
        currentRecords = nullptr;
    }
    
    //return true if at least One valid record was read in False otherwise
    //greater than one because header was included in line count
    return currentLineCount > 1;
}
//parses line of record data and stores it into a NvraRecord object
NvraRecord* readRecord(string recordInfo, int& currentLineCount)
{
    //Recored for each valid line of data
    NvraRecord* currentRecord = new NvraRecord;
    if(currentRecord == NULL)
        throw new ExceptionMemoryNotAvailable;
    
    string data = "";
    
    //learned how to parse cvs files using stringstream at https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
    stringstream stream(recordInfo);
    //Parses line and retrives all values
    for(int i = 0; i < 24 && recordInfo.length() > 1; ++i)
    {
        getline(stream, data, ',');
        
        if (i == 3 || i == 11 || i == 12)
        {
            currentRecord -> addString(data);
        }
        else
        {
            int dataValue = stoi(data);
            // Checks for invalid data
            if(dataValue < 0)
            {
                cout << "Invalid data at line " + to_string(currentLineCount) + "." << endl;
                return nullptr;
            }
            else
            {
                currentRecord -> addNum(dataValue);
            }
        }
    }
    
    return currentRecord;
}



//Used to locate NvraRecords whithin NvraArray
void find(struct filesSummary* dataInfo)
{
    cout << "Enter sort field (0-23): " << endl;;
    string sortField;
    getline(cin, sortField);
    int searchCollum = isValidSortField(sortField);
    
    if(searchCollum == -1)
    {
        cout << " data Collum was --1" << endl;
    }
    else if(searchCollum == 3 || searchCollum == 11 || searchCollum == 12)
    {
        searchForStringData(dataInfo, searchCollum);
    }
    else
    {
        searchForNumericData(dataInfo, searchCollum);
    }
}

void searchForNumericData(filesSummary* dataInfo, int searchCollum)
{
    cout << "Enter non-negative field value: " << endl;
    string response;
    getline(cin, response);
    
    int data;
    try
    {
        data = stoi(response);
        if(data < 0)
        {
            data = data * -1;
        }
    } catch (exception invalid_argument)
    {
        return;             //incase the user enters an a non integer for the collum to be sorted
    }
    
    NvraRecord compareToRecord;
    compareToRecord.setNum(data, searchCollum);

    if(searchCollum == 0)
    {
        try
        {
            NvraRecord record = dataInfo -> hashTable -> find(&compareToRecord);
            cout << record << "\nNVRA records found: " << 1 << "." <<endl;
        }
        catch (ExceptionHashTableAccess* e)
        {
            delete e;
            cout << "NVRA records found: " << 0 << "." <<endl;
        }
    }
    else
    {
        NvraComparator comparator(searchCollum);
        if(searchCollum == dataInfo -> sortedOn)
        {
            long long location = binarySearch(compareToRecord, *dataInfo -> nvraArray,comparator);
            
            if(location >= 0)
            {
                unsigned long index = static_cast<unsigned long>(location);
                unsigned long recordsFound = 0;
                //prints all records that that comply with the search request
                for(unsigned long i = index; i < dataInfo -> nvraArray -> getSize() && comparator.compare(dataInfo -> nvraArray -> get(i), compareToRecord) == 0; ++i, ++recordsFound)
                {
                    cout << dataInfo -> nvraArray -> get(i) << endl;
                }
                cout << "NVRA records found: " << recordsFound << "." <<endl;
            }
        }
        else
        {
            linearSearch(dataInfo -> nvraArray, comparator, compareToRecord);
        }
    }
}

void searchForStringData(filesSummary* dataInfo, int searchCollum)
{
    cout << "Enter exact text on which to search: ";
    string response;
    getline(cin, response);
    
    if(response.length() == 0)
    {
        return;
    }
    else
    {
        //Makes record with info equivalent to what the user is searching for
        NvraRecord compareToRecord;
        compareToRecord.setString(response, searchCollum);
        
        NvraComparator comparator(searchCollum);
        //if already sorted on correct collum binary search is used
        if(searchCollum == dataInfo -> sortedOn)
        {
            long long location = binarySearch(compareToRecord, *dataInfo -> nvraArray,comparator);
            if(location >= 0)
            {
                unsigned long index = static_cast<unsigned long>(location);
                unsigned long recordsFound = 0;
                //prints all records that that comply with the search request
                for(unsigned long i = index; i < dataInfo -> nvraArray -> getSize() && comparator.compare(dataInfo -> nvraArray -> get(i), compareToRecord) == 0; ++i, ++recordsFound)
                {
                    cout << dataInfo -> nvraArray -> get(i) << endl;
                }
                cout << "NVRA records found: " << recordsFound << "." <<endl;
            }
        }
        else
        {
            linearSearch(dataInfo -> nvraArray, comparator, compareToRecord);
        }
    }
}

//Used to Sort records
void sort(struct filesSummary* nvraArray)
{
    cout << "Enter sort field (0-23): ";
    string sortField;
    getline(cin, sortField);
    
    ///////test
    int sortCollum = isValidSortField(sortField);
    if(sortCollum != -1)
    {
        NvraComparator comparator(sortCollum);
        Sorter<NvraRecord>::sort(*nvraArray -> nvraArray, comparator);
        nvraArray -> sortedOn = sortCollum;
    }
}

//Verifies user inputed data is with in collum range
int isValidSortField(string sortField)
{
    try
    {
        int dataCollum = stoi(sortField);
        if(dataCollum >= 0 && dataCollum < 24)
            return dataCollum;
    } catch (exception invalid_argument)
    {
        /*incase the user enters an a non integer for the collum to be sorted*/
    }
    return -1;
}

void linearSearch(TemplatedArray<NvraRecord>* array, NvraComparator comparatror, NvraRecord value)
{
    unsigned long recordsFound = 0;
    for(unsigned long index = 0; index < array -> getSize(); ++index)
    {
        if(comparatror.compare(array -> get(index), value) == 0)
        {
            cout << array -> get(index) << endl;
            ++recordsFound;
        }
    }
    cout << "NVRA records found: " << recordsFound << "." <<endl;
}

//Used to output record Data
void output(TemplatedArray<NvraRecord>* nvraArray, unsigned long validLineCount, unsigned long lineCount)
{
    cout << "Enter output file name: ";
    string fileName = "";
    getline(cin, fileName);
    ofstream myfile;
    myfile.open(fileName);
    //If user request for data to be printed on a file
    if(fileName.length() > 0)
    {
        while(!myfile.is_open() && fileName.length() > 0)
        {
            cout << "File is not available.\nEnter output file name: ";
            getline(cin, fileName);
            myfile.open(fileName);
        }
        if(fileName.length() > 0 && nvraArray -> getSize() > 0)
        {
            for(unsigned long i = 0; i < nvraArray -> getSize(); ++i)
            {
                //makes sure data being outputed has info
                if(nvraArray -> get(i).getString(1).length() > 1)
                    myfile << nvraArray -> get(i) << endl;
            }
            myfile << "Data lines read: " << lineCount - 1<< "; Valid NVRA records read: " << validLineCount << "; NVRA records in memory: " << nvraArray -> getSize() << endl;
            myfile.close();
        }
        else
        {
            myfile << "Data lines read: " << lineCount - 1<< "; Valid NVRA records read: " << validLineCount << "; NVRA records in memory: " << nvraArray -> getSize() << endl;
            myfile.close();
        }
        
    }
    else//prints to console
    {
        for(unsigned long i = 0; i < nvraArray -> getSize(); ++i)
        {
            //makes sure data being outputed has info
            if(nvraArray -> get(i).getString(1).length() > 1)
                cout << nvraArray -> get(i) << endl;
        }
        cout << "Data lines read: " << lineCount - 1<< "; Valid NVRA records read: " << validLineCount << "; NVRA records in memory: " << nvraArray -> getSize() << endl;
    }
}

void displayHashTable(HashTable<NvraRecord>* hashTable, AVLTree<NvraRecord>* tree, unsigned long validLineCount, unsigned long lineCount)
{
    
    HashTableEnumerator<NvraRecord> enumerator(hashTable);
    unsigned long currentBucketNumber = 0;
    
    cout << "Enter output file name: ";
    string fileName = "";
    getline(cin, fileName);
    
    //If user request for data to be printed on a file
    if(fileName.length() > 0 && hashTable -> getSize() > 0)
    {
        ofstream myfile;
        myfile.open(fileName);
        while(!myfile.is_open() && fileName.length() > 0)
        {
            cout << "File is not available.\nEnter output file name: ";
            getline(cin, fileName);
            myfile.open(fileName);
        }
        
        while(enumerator.hasNext())
        {
            NvraRecord record = enumerator.next();
            unsigned long currentBucket = hashTable -> getBucketNumber(&record);
            
            myfile << currentBucket << ": " << record << endl;
            currentBucketNumber = currentBucket;
            
            bool exit = false;
            while(enumerator.hasNext() && !exit)
            {
                record = enumerator.peek();
                currentBucket = hashTable -> getBucketNumber(&record);
                if(currentBucket == currentBucketNumber)
                {
                    record = enumerator.next();
                    myfile << "OVERFLOW: " << record << endl;
                }
                else
                {
                    exit = true;
                }
            }
            myfile << endl;
        }
        myfile << "Base Capacity: " << hashTable -> getBaseCapacity() << "; Total Capacity: " << hashTable -> getTotalCapacity() << "; Load Factor: " << hashTable -> getLoadFactor() << endl;
        
        myfile << dataSummery(tree, validLineCount, lineCount) << endl;
        myfile.close();
        
    }
    else//prints to console
    {
        while(enumerator.hasNext())
        {
            NvraRecord record = enumerator.next();
            unsigned long currentBucket = hashTable -> getBucketNumber(&record);
            
            cout << currentBucket << ": " << record << endl;
            currentBucketNumber = currentBucket;
            
            bool exit = false;
            while(enumerator.hasNext() && !exit)
            {
                record = enumerator.peek();
                currentBucket = hashTable -> getBucketNumber(&record);
                if(currentBucket == currentBucketNumber)
                {
                    record = enumerator.next();
                    cout << "OVERFLOW: " << record << endl;
                }
                else
                {
                    exit = true;
                }
            }
            cout << endl;
        }
        cout << "Base Capacity: " << hashTable -> getBaseCapacity() << "; Total Capacity: " << hashTable -> getTotalCapacity() << "; Load Factor: " << hashTable -> getLoadFactor() << endl;
        
        cout << dataSummery(tree, validLineCount, lineCount) << endl;
    }
}

void printTree(filesSummary* dataInfo, AVLTreeOrder order)
{
    AVLTreeEnumerator<NvraRecord> enumerator(dataInfo -> tree, order);
    
    cout << "Enter output file name: ";
    string fileName = "";
    getline(cin, fileName);
    
    //If user request for data to be printed on a file
    if(fileName.length() > 0)
    {
        ofstream myfile;
        myfile.open(fileName);
        while(!myfile.is_open() && fileName.length() > 0)
        {
            cout << "File is not available.\nEnter output file name: ";
            getline(cin, fileName);
            myfile.open(fileName);
        }
        
        while(enumerator.hasNext())
        {
            myfile << enumerator.next() << endl;
        }
        
        myfile << dataSummery(dataInfo -> tree, dataInfo -> validLinecount, dataInfo -> totalLineCount) << endl;

        myfile.close();
        
    }
    else//prints to console
    {
        while(enumerator.hasNext())
        {
            cout << enumerator.next() << endl;
        }
        cout << dataSummery(dataInfo -> tree, dataInfo -> validLinecount, dataInfo -> totalLineCount) << endl;
    }
    
}


string dataSummery(AVLTree<NvraRecord>* tree, unsigned long validLineCount, unsigned long lineCount)
{
    return "Data lines read: " + to_string(lineCount - 1) + "; Valid NVRA records read: " + to_string(validLineCount)
    + "; NVRA records in memory: " + to_string(tree -> getSize());
}

void updateFiles(filesSummary* dataInfo)
{
    if(dataInfo -> nvraArray != NULL)
    {
        delete dataInfo -> nvraArray;
        dataInfo -> nvraArray = NULL;
        
        delete dataInfo -> hashTable;
        dataInfo -> hashTable = NULL;
    }
    
    dataInfo -> nvraArray = new TemplatedArray<NvraRecord>(dataInfo -> tree -> getSize());
    dataInfo -> hashTable = new HashTable<NvraRecord>(new NvraComparator(0), new NvraHasher);
    
    if(dataInfo -> tree -> getSize() != 0)
    {
        AVLTreeEnumerator<NvraRecord> enumerator(dataInfo -> tree, AVLTreeOrder::inorder);
        
        while(enumerator.hasNext())
        {
            NvraRecord record = enumerator.next();
            dataInfo -> hashTable -> insert(&record);
            dataInfo -> nvraArray -> add(&record);
        }
    }
}

