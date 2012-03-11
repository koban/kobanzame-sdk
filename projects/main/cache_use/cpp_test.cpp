/**
 *	@file	cpp_test.cpp
 *	@brief	KOBANZAME SDK C++ Test code
 *
 *	KOBANZAME SDK
 *		Software Developers Kit for Blackfin DSP Evaluation Board(KOBANZAME).
 *		
 *	Copyright (C) 2010, KOBANZAME SDK Project, all right reserved
 *
 *	LICENSE:
 *	The software is a free and you can use and redistribute it for
 *	personal, non-profit or commercial products. Redistributions of
 *	source code must retain the above copyright notice. There is no 
 *	warranty in this software, if you suffer any damages by using 
 *	the software.
 */
 
 
 /*--- include -----------------------------------*/
#include "kobanzame.h"

class CTestSuper {
	
public:
	int mValue;


	CTestSuper()
	{
		mValue = 100;
	}
	virtual ~CTestSuper()
	{
		printf("CTestSuper dtor\n");
	}
	
	virtual void Print( )
	{
		printf("CTestSuper::Print %d\n", mValue);
	}
	void PrintAddr()
	{
		printf("ObjAddr: 0x%08X\n",(long)this );
	}
};


class CTestChildA : public CTestSuper {
	
public:
	CTestChildA()
	{
		mValue = 200;
	}
	virtual ~CTestChildA()
	{
		printf("CTestChildA dtor\n");
	}
	virtual void Print( )
	{
		printf("CTestChildA::Print %d\n", mValue);
	}
};

 /*--- Global -----------------------------------*/

CTestSuper	gSuper;
CTestChildA gChildA;

extern "C"
void CppTest()
{
	gSuper.PrintAddr();
	gSuper.Print();
	gChildA.PrintAddr();
	gChildA.Print();	
}

extern "C"
void CppNewTest()
{
	CTestSuper *super = new CTestSuper;
	super->PrintAddr();
	super->Print();
	delete super;
	
	CTestChildA *child = new CTestChildA;
	child->PrintAddr();
	child->Print();
	delete child;
	
}


extern "C"
L3CODE
void HeapTest(UW bytes)
{	
	B *heap = new B[bytes];
	
	if( heap == 0 )
	{
		printf("Heap Cannot Allocate\n");
	}
	else
	{
		B test_dat = 0;
		const B test_end_val = 77;
		for( UW i=0;i<bytes;++i )
		{
			heap[i] = test_dat++;
			if( test_dat == test_end_val) test_dat = 0;
		}
		
		// Verify
		test_dat = 0;
		for( UW i=0;i<bytes;++i )
		{
			if( heap[i] != test_dat++ )
			{
				printf("Heap Verify Error\n");
				delete[] heap;
				return;
			}
			if( test_dat == test_end_val) test_dat = 0;
		}
		
		delete[] heap;
		printf(" Heap Test OK\n");
	}
	
}
