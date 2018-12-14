#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <conio.h>
//#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
//-----------------------------------------------------------------------------
const unsigned short int g_usMaxCntAuthorSymbols = 31;/*максимальна кількість символів
                                       в назві автора книги - 30*/
const unsigned short int g_usMaxCntTitleSymbols = 51;/*максимальна кількість символів y
									   назві книги - 50*/
const unsigned short int g_usMaxYearPublic = 2017;
const unsigned short int g_usMinYearPublic = 1000;
const unsigned short int g_usMaxCntPages = 10000;
const unsigned short int g_usMinCntPages = 5;
const float g_fMaxCost = 10000.0;
const float g_fMinCost = 0.0;
struct SbookData {
	char sAuthor[g_usMaxCntAuthorSymbols];
	char sTitle[g_usMaxCntTitleSymbols];
	unsigned short usYearPublication;
	unsigned short usPagesCnt;
	float fCost;
};

//-----------------------------------------------------------------------------

struct SbookItem {
	SbookData Book;
	SbookItem *pNextBook;
};

//-----------------------------------------------------------------------------

void mainMenu();
void addBookToEnd(SbookItem **ppHeadListBooks);
void printBooksData(SbookItem *pHeadListBooks);
void initBookData(SbookItem **ppNewBook);
void sortListBooksByYearPublic(SbookItem *pHeadListBooks);
void sortListBooksByAuthorName(SbookItem *pHeadListBooks);
void readWithFile(SbookItem **ppHeadListBooks);
void defThreeBooksSmallestCntPages(SbookItem *pHeadListBooks);
void deleteBooksAuthorsNameBeginningletter_J(SbookItem **ppHeadListBooks);
unsigned calcNumberBooks(SbookItem *pHeadListBooks);
void copyListToArray(SbookData *arrayBook,SbookItem *pHeadListBooks,const unsigned uBookCnt);
void Swap(SbookData *left,SbookData *right);
void writeToFile(SbookItem *pHeadListBooks);
bool checkStrAuthor(char *str);
bool checkStrTitleBook(char *str);
bool checkStrIntVal(char *str);
bool checkStrFloatVal(char *str);
bool checkSpace(char *str);
inline void message();
inline void clearBuff();
//-----------------------------------------------------------------------------

int main()
{
	SbookItem *pHeadListBooks = NULL;/*Вказівник на перший елемент 
									 однозв'язного списку книжок*/
	readWithFile(&pHeadListBooks);
	bool bProgramWorking = true;
	while (bProgramWorking == true)
	{
		system("cls");
		mainMenu();
		char chKey = _getch();
			switch (chKey)
			{
			    case '1':	
					printBooksData(pHeadListBooks); 
					break;//
				case '2':	
					addBookToEnd(&pHeadListBooks); 
					break;//
				case '3':	
					sortListBooksByYearPublic(pHeadListBooks); 
					break;//
				case '4':   
					defThreeBooksSmallestCntPages(pHeadListBooks); 
					break;//
				case '5':	
					deleteBooksAuthorsNameBeginningletter_J(&pHeadListBooks);
					break;//видалення
				case '6':	
					sortListBooksByAuthorName(pHeadListBooks);
					break;//Сортування
				case '7':	
					writeToFile(pHeadListBooks);
					break;//Запис у файл
				case 'e':
				  case 'E':
				    case 'У':
		                 bProgramWorking = false;
					  break;//Завершення роботи програми
				default:break;
			}
	}
	system("pause");
	return 0;
}

//-----------------------------------------------------------------------------
//Функція виводить в консоль головне меню
void mainMenu()
{
	cout << "Main menu :\n";
	cout << "1. Display books. \n";
	cout << "2. Add a book to end list.\n";
	cout << "3. Sorting the list of books by year of publication in order of increasing.\n";
	cout << "4. Define three books with the smallest number of pages.\n";
	cout << "5. Remove data about books whose author's name begins with the letter 'J'.\n";
	cout << "6. Sorting the list of books by the author's name in lexicographical order. \n";
	cout << "7. Write to file <tableBooks.txt>.\n";
	cout << "Press 'e' to end program.\n";
	cout << " \n";
	return;
}

//-------------------------------------------------------------------------------
//Функція виводить дані про книги у вигляді таблиці у файл tableBooks.txt
//Формат таблиці : 
//номер книги |Назва автора | Назва книги | рік видання | кількість сторінок | ціна
//Якщо файл tableBooks.txt не вдалось відкрити , то виводиться відповідне повідомлення
void writeToFile(SbookItem *pHeadListBooks)
{
	system("cls");//якщо пустй список,то не таблиця
	FILE *ptrFile = fopen("tableBooks.txt", "w");
	if (ptrFile == NULL) {
		perror("Error open the file tableBooks.txt ");
		cout << "Press any key to return to the main menu\n";
		_getch();
		_getch();
		return;
	}
	int iNumber = 1;
	fprintf(ptrFile,"___________________________________________________________");
	fprintf(ptrFile, "__________________________________________________________\n");
	fprintf(ptrFile, "%-2s||\t\t\t%-20s||\t\t\t\t%-38s", "#", "Author", "Name of the book");
	fprintf(ptrFile, "|| %s || %s ||%s\n", "Year", "Pages", "Cost");
	fprintf(ptrFile, "==||================================||=====================================");
	fprintf(ptrFile, "===============||======||=======||=======\n");
	SbookItem *pCurrent = pHeadListBooks;
	while (pCurrent != NULL) {
		fprintf(ptrFile, "%-2d|| %-30s || %-50s ",
			iNumber, pCurrent->Book.sAuthor, pCurrent->Book.sTitle);
		fprintf(ptrFile, "|| %-4hu || %-4hu  || %-4.2f\n",
			pCurrent->Book.usYearPublication, pCurrent->Book.usPagesCnt, pCurrent->Book.fCost);
		++iNumber;
		pCurrent = pCurrent->pNextBook;
	}
	fprintf(ptrFile,"_____________________________________________________________");
	fprintf(ptrFile,"________________________________________________________\n\n");
	fclose(ptrFile);
	cout << "Data about books is successfully written to a file <tableBooks.txt>\n";
	cout << "Press any key to return to the main menu\n";
	_getch();
	_getch();
	return;
}
//-----------------------------------------------------------------------------
//Функція виводить дані про книги у вигляді таблиці на екран
//Якщо список пустий ,то виводиться відповідне повідомлення
//Формат таблиці : 
//номер книги |Назва автора | Назва книги | рік видання | кількість сторінок | ціна
void printBooksData(SbookItem *pHeadListBooks)
{
	system("cls");
	if (pHeadListBooks ==  NULL)
	{
		cout << "No data about books\n";
		cout << "Press any key to return to the main menu\n";
		_getch();
		return;
	}
	SbookItem *pCurrentPrint = pHeadListBooks;
	int iNumberBooks = 1;
	printf("%-50sList books\n"," ");
	printf("___________________________________________________________");
	printf("____________________________________________________________\n");
	printf("%-2s||\t\t%-20s||\t\t  %-40s", "#", "Author", "Name of the book");
	printf("|| %s || %s  ||%s\n", "Year", "Pages", "Cost");
	printf("==||================================||=====================================");
	printf("===============||======||========||=========\n");
	while (pCurrentPrint){
		printf("%-2d|| %-30s || %-50s ",iNumberBooks,pCurrentPrint->Book.sAuthor, pCurrentPrint->Book.sTitle);
		printf("|| %-4hu || %-5hu  || %-4.2f\n", pCurrentPrint->Book.usYearPublication,
			pCurrentPrint->Book.usPagesCnt, pCurrentPrint->Book.fCost);
		pCurrentPrint = pCurrentPrint->pNextBook; 
		++iNumberBooks;
	} 
	printf("___________________________________________________________");
	printf("____________________________________________________________\n\n");
	cout << "Press any key to return to the main menu\n";
	//_getch();
	system("pause");
	return;
}

//-------------------------------------------------------------------------------------------------------------------
//Функція ініціалізовує дані про книгу,які вводяться з клавіатури
//SbookItem **ppNewBook - вказівник на вказівник на нову книгу
void initBookData(SbookItem **ppNewBook)
{
	system("cls");
	const int iCntMaxStrSymbols = 100;//Поміняти на 500
	char str[iCntMaxStrSymbols];
	int iValue;
	float fValue;
	while (true) {
		cout << "Enter name author : ";
		cin.getline(str, iCntMaxStrSymbols);
		if (strlen(str) > g_usMaxCntAuthorSymbols) {
			printf("Wrong!Number of characters in the author's name is over %d\n",
				g_usMaxCntAuthorSymbols - 1);
			clearBuff();
			continue;
		}
		if (checkStrAuthor(str) == false) {
			cout << "Wrong!In the author's name there are inadmissible characters or line is empty\n";
		}
		else {
			strcpy((*ppNewBook)->Book.sAuthor, str);
			break;
		}
	}
	//-----------------------------------------------------------------------------------------------------
	while (true) {
		cout << "Enter name of the book : ";
		cin.getline(str, iCntMaxStrSymbols);
		if (strlen(str) > g_usMaxCntTitleSymbols) {
			printf("Wrong!Number of characters in the title book is over %d\n",
				g_usMaxCntTitleSymbols - 1);
			clearBuff();
			continue;
		}
		if (checkStrTitleBook(str) == false) {
			cout << "Wrong!In the title book there are inadmissible characters or line is empty\n";
		}
		else {
			strcpy((*ppNewBook)->Book.sTitle, str);
			break;
		}
	}
	//--------------------------------------------------------------------------------------------------
	while (true) {
		cout << "Enter the publication year of the book : ";
		cin.getline(str, iCntMaxStrSymbols);
		clearBuff();
		if (checkStrIntVal(str) == false) {
			cout << "Wrong!In the year publication are inadmissible characters or line is empty\n";
			continue;
		}
		iValue = strtol(str, NULL, 10);
		if (iValue < g_usMinYearPublic || iValue > g_usMaxYearPublic) {
			printf("%d < year publications < %d\n", g_usMinYearPublic, g_usMaxYearPublic);
		}
		else {
			(*ppNewBook)->Book.usYearPublication = iValue;
			break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	while (true) {
		cout << "Enter the number of pages in the book : ";
		cin.getline(str, iCntMaxStrSymbols);
		clearBuff();
		if (checkStrIntVal(str) == false) {
			cout << "Wrong!In the  number of pages book there are inadmissible characters or line is empty!\n";
			continue;
		}
		iValue = strtol(str, NULL, 10);
		if (iValue < g_usMinCntPages || iValue > g_usMaxCntPages) {
			printf("%d < Pages < %d\n", g_usMinCntPages, g_usMaxCntPages);
		}
		else {
			(*ppNewBook)->Book.usPagesCnt = iValue;
			break;
		}
	}
	//------------------------------------------------------------------------------------------------
	while (true) {
		cout << "Enter the cost of the book : ";
		cin.getline(str, iCntMaxStrSymbols);
		clearBuff();
		if (checkStrFloatVal(str) == false) {
			cout << "Wrong!In the cost book there are inadmissible characters or line is empty!\n";
			continue;
		}
		fValue = strtod(str, NULL);
		if (fValue < g_fMinCost || fValue > g_fMaxCost) {
			printf("%.2f < cost < %.2f\n", g_fMinCost, g_fMaxCost);
		}
		else {
			(*ppNewBook)->Book.fCost = fValue;
			break;
		}
	}
	return;
}

//------------------------------------------------------------------------------
//Функція додає книгу в кінець однозв'язного списку
//і записує дані про книжку в кінець файлу Books.txt
//Якщо файл не вдалось відкрити ,то  
//SbookItem **ppHeadListBooks - вказівник на вказівник на початок однозв'язного списку
void addBookToEnd(SbookItem **ppHeadListBooks)
{
	SbookItem *pCurrent = nullptr;
	//Поставити прапор для того ,щоб знати скільки рядків пропускати у файлі
	if (*ppHeadListBooks == NULL){
		*ppHeadListBooks = new SbookItem;
		initBookData(ppHeadListBooks);
		(*ppHeadListBooks)->pNextBook = NULL;
		pCurrent = *ppHeadListBooks;
	}
	else{
		pCurrent = *ppHeadListBooks;
		while (pCurrent->pNextBook != NULL)
			pCurrent = pCurrent->pNextBook;
		SbookItem *pNewBook = new SbookItem;
		initBookData(&pNewBook);
		pNewBook->pNextBook = NULL;
		pCurrent->pNextBook = pNewBook;
		pCurrent = pCurrent->pNextBook;
	} 
	cout << "\nWrite to file <Books.txt>? : Y/n ";
	char chKey = _getch();
	if (chKey == 'Y' || chKey == 'y') {
		FILE *ptrFile = fopen("Books.txt", "a");
		if (ptrFile == NULL) {
			perror("Error open the file <Books.txt> ");
			_getch();
			return;
		}
		fprintf(ptrFile, "\n%s\n%s\n%hu\n%hu\n%.1f\n", pCurrent->Book.sAuthor,
			pCurrent->Book.sTitle, pCurrent->Book.usYearPublication,
			pCurrent->Book.usPagesCnt, pCurrent->Book.fCost);
		fclose(ptrFile);
		cout << "\nData about the book successfully written to a file <Books.txt>\n";
	}
	else
		cout << "\nRejected\n";
	cout << "Press any key to return to the main menu\n";
	_getch();
	return;
}

//-----------------------------------------------------------------------------
//Функція сортує  список книг  в порядку зростання за роком видання
//SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
//Відсортовані книги виводяться на екран у вигляді таблиці
//Для виводу на екран використовується функція 
//void printBooksData(SbookItem *pHeadListBooks)
void sortListBooksByYearPublic(SbookItem *pHeadListBooks)
{
	if (pHeadListBooks == NULL) 
		return; 
	const unsigned uBookCnt = calcNumberBooks(pHeadListBooks);
	SbookItem *pCurrent = pHeadListBooks;
	for (int j = 0; j < uBookCnt; ++j) {
		for (int i = 1; i < uBookCnt; ++i) {
			SbookItem *pNext = new SbookItem;
			pNext = pCurrent->pNextBook;
			if ((pCurrent->Book.usYearPublication) > (pNext->Book.usYearPublication)) {
				SbookItem *tmp = new SbookItem;
				SbookItem *tmpPtr;
				*tmp = *pCurrent;
				*pCurrent = *pNext;
				*pNext = *tmp;
				tmpPtr = pCurrent->pNextBook;
				pCurrent->pNextBook = pNext->pNextBook;
				pNext->pNextBook = tmpPtr;
			}
			pCurrent = pNext;
		}
		pCurrent = pHeadListBooks;
	}
	cout << "Books after sort\n";
	printBooksData(pHeadListBooks);
}

//-----------------------------------------------------------------------------
//Функція сортує список книг в лексикографічному порядку за ім'ям автора
//Відсортовані книги виводяться на екран у вигляді таблиці
//Для виводу на екран використовується функція 
//void printBooksData(SbookItem *pHeadListBooks)
//SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
void sortListBooksByAuthorName(SbookItem *pHeadListBooks)
{
	if (pHeadListBooks == NULL)
		return;
	const unsigned uBookCnt = calcNumberBooks(pHeadListBooks);
	SbookItem *pCurrent = pHeadListBooks;
	for (int j = 0; j < uBookCnt; ++j) {
		for (int i = 1; i < uBookCnt; ++i) {
			SbookItem *pNext = new SbookItem;
			pNext = pCurrent->pNextBook;
			if (strncmp(pCurrent->Book.sAuthor ,pNext->Book.sAuthor, g_usMaxCntAuthorSymbols) > 0) {
				SbookItem *tmp = new SbookItem;
				SbookItem *tmpPtr;
				*tmp = *pCurrent;
				*pCurrent = *pNext;
				*pNext = *tmp;
				tmpPtr = pCurrent->pNextBook;
				pCurrent->pNextBook = pNext->pNextBook;
				pNext->pNextBook = tmpPtr;
			}
			pCurrent = pNext;
		}
		pCurrent = pHeadListBooks;
	}
	cout << "Books after sort\n";
	printBooksData(pHeadListBooks);
}

//-----------------------------------------------------------------------------
//Функція обчислює кількість книжок у списку
// SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
//Функція повертає значення кількості книжок у списку
unsigned calcNumberBooks(SbookItem *pHeadListBooks)
{
	SbookItem *pCurrent = pHeadListBooks;
	unsigned uBookCnt = 0;
	while (pCurrent != NULL) {
		pCurrent = pCurrent->pNextBook;
		uBookCnt++;
	}
	return uBookCnt;
}
 
//-----------------------------------------------------------------------------
//Функція зчитує з текстового файлу Books.txt дані про книги.
//Формат файлу Books.txt:
//Назва автора
//Назва книги
//Рік видання
//Кількість сторінок
//Ціна
// SbookItem **ppHeadListBooks - вказівник на вказівник на початок однозв'язного списку
//Якщо файл не вдалось відкрити ,то виводиться відвовідне повідомлення
void readWithFile(SbookItem **ppHeadListBooks)
{
	FILE *ptrFile = fopen("Books.txt", "r");///////////////////////////////////////////////
	if (ptrFile == NULL) {
		perror("Error open the file <Books.txt> ");
		cout << "Press any key to continue program\n";
		_getch();
		return;
	}
	fseek(ptrFile, 0, SEEK_END);
	if (ftell(ptrFile) == 0) {
		fclose(ptrFile);
		cout << "File <Books.txt> is empty\n";
		cout << "Press any key to continue program\n";
		_getch();
		return;
	}
	rewind(ptrFile);
	SbookItem *pCurrent = new SbookItem;
	int iValue = 0;
	float fValue = 0;
	int iCurrentString = 0;
	const int iCntMaxStrSymbols = 100;
	char str[iCntMaxStrSymbols];
	while (!feof(ptrFile))
	{
		if (*ppHeadListBooks == NULL) {
		*ppHeadListBooks = pCurrent;
			(*ppHeadListBooks)->pNextBook = NULL;
		}
		else {
			SbookItem *p = new SbookItem;
			pCurrent->pNextBook = p;
			pCurrent = p;
		}
	//-----------------------------------------------------------------------------
		fgets(str,iCntMaxStrSymbols, ptrFile);//Зчитування з файлу
		iCurrentString++;
		if (strlen(str) > g_usMaxCntAuthorSymbols) {
			printf("Invalid data format in file <Books.txt> : ");
				printf("number of characters in the author's name is over %d,\nString# %d\n",
				g_usMaxCntAuthorSymbols - 1,iCurrentString);
				message();
		}
		if (checkStrAuthor(str) == false) {
			cout << "Invalid data format in file <Books.txt> : ";
			cout << "in the author's name there are inadmissible characters"; 
			cout << "\nor the line is empty,\nString# " << iCurrentString << endl;
			message();
		}
		strcpy(pCurrent->Book.sAuthor, str);
		*str = 0;
		if (!feof(ptrFile)) {
			fgets(str, iCntMaxStrSymbols, ptrFile);
			iCurrentString++;
		}
		if (strlen(str) > g_usMaxCntTitleSymbols) {
			printf("Invalid data format in file <Books.txt> :");
			printf("number of characters in the title book is over %d,\nString# %d\n",
				g_usMaxCntTitleSymbols - 1,iCurrentString);
			message();
		}
		if (checkStrTitleBook(str) == false) {
			cout << "Invalid data format in file <Books.txt> : ";
			cout << "in the title book there are inadmissible characters";
			cout << "\nor the line is empty\nString# " 
				<< iCurrentString << endl;
			message();
		}
		strcpy(pCurrent->Book.sTitle, str);
		//рік публікації--------------------------------------------------------------------------------
		*str = ' ';
		if (!feof(ptrFile)) {
			fgets(str, iCntMaxStrSymbols, ptrFile);
			iCurrentString++;
		}
		if (checkStrIntVal(str) == false) {
			cout << "Invalid data format in file <Books.txt> : ";
			cout << "in the year publication are inadmissible characters or line is empty\nstring# "
				<< iCurrentString << endl;
			message();
		}
		iValue = strtol(str,NULL,10);
		if (iValue < g_usMinYearPublic || iValue > g_usMaxYearPublic){
			cout << "Invalid data format in file <Books.txt> : ";
			printf("%d < year publications < %d\nString# %d\n", g_usMinYearPublic, g_usMaxYearPublic,iCurrentString);
			message();
		}
		pCurrent->Book.usYearPublication = iValue;
	    //сторінки-----------------------------------------------------------------------------------------
		*str = ' ';
		if (!feof(ptrFile)) {
			fgets(str, iCntMaxStrSymbols, ptrFile);
			iCurrentString++;
		}
		if (checkStrIntVal(str) == false) {
			cout << "Invalid data format in file <Books.txt> : ";
			cout << "in the count pages are inadmissible characters or line is empty,\nString# " 
				<< iCurrentString << endl;
			message();
		}
		iValue = strtol(str, NULL, 10);
		if (iValue < g_usMinCntPages || iValue > g_usMaxCntPages) {
			cout << "Invalid data format in file <Books.txt> : ";
			printf("%d < Pages < %d\nString# %d\n",g_usMinCntPages,g_usMaxCntPages,iCurrentString);
			message();
		}
		pCurrent->Book.usPagesCnt = iValue;
		//ціна--------------------------------------------------------------------------------------
		*str = ' ';
		if (!feof(ptrFile)) {
			fgets(str, iCntMaxStrSymbols, ptrFile);
			iCurrentString++;
		}
		if (checkStrFloatVal(str) == false) {
			cout << "Invalid data format in file <Books.txt> : ";
			cout << "in the cost are inadmissible characters or line is empty,\nString# "
				<< iCurrentString << endl;
			message();
		}
		fValue = strtod(str, NULL);
		if (fValue < g_fMinCost || fValue > g_fMaxCost) {
			cout << "Invalid data format in file <Books.txt> : ";
			printf("%.2f < cost < %.2f\nString# %d\n", g_fMinCost,g_fMaxCost,iCurrentString);
			message();
		}
		pCurrent->Book.fCost = fValue;
		//--------------------------------------------------------------------------------------------------
		fgets(str,iCntMaxStrSymbols, ptrFile);
		iCurrentString++;
		if (checkSpace(str) == false && !feof(ptrFile)) {
			cout << "Invalid data format in file <Books.txt> :\nString# " 
				<< iCurrentString << endl;
			message();
		}
		for (int j = 0; j < g_usMaxCntTitleSymbols; j++) {
			if (pCurrent->Book.sAuthor[j] == '\n')
				pCurrent->Book.sAuthor[j] = '\0';
			if (pCurrent->Book.sTitle[j] == '\n')
				pCurrent->Book.sTitle[j] = '\0';
		}
	}
	pCurrent->pNextBook = NULL;
	fclose(ptrFile);
	return;
}

//------------------------------------------------------------------------------
//Функція визначає три книги з найменшою кількістю сторінок і виводить їх на екран 
// у форматі  : назва автора  назва книги - кількість сторінок
//Якщо у списку менше трьох книг,то виводьться всі книги ,які є у списку
//SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
//Якщо список пустий ,то виводиться відповідне повідомлення
void defThreeBooksSmallestCntPages(SbookItem *pHeadListBooks)
{
	system("cls");
	if (pHeadListBooks == NULL) {
		cout << "No data about books\n";		
		return;
	}
	const unsigned uBookCnt = calcNumberBooks(pHeadListBooks);
	unsigned N;
	if (uBookCnt < 3) {
		N = uBookCnt;
	}
	else {
		N = 3;
	}
	SbookData *arrayBooks = new SbookData[uBookCnt];
	copyListToArray(arrayBooks,pHeadListBooks,uBookCnt);
	unsigned short minPages;
	for (int i = 0; i < N; ++i)
	{
		minPages = arrayBooks[i].usPagesCnt;
		for (int j = i + 1; j < uBookCnt; ++j)
		{
			if (arrayBooks[i].usPagesCnt < minPages) {
				minPages = arrayBooks[i].usPagesCnt;
				Swap(&arrayBooks[i], &arrayBooks[j]);
			}
		}
	}
	printf("Three books with the smallest number of pages : \n\n");
	for (int i = 0; i < N; ++i){
		printf("%d\t%-30s%-30s\t%hu pages\n", i + 1, arrayBooks[i].sAuthor, 
			arrayBooks[i].sTitle, arrayBooks[i].usPagesCnt); 
	}
	printf("\nPress any key to return to the main menu\n");
	delete[] arrayBooks;
	arrayBooks = NULL;
	clearBuff();
	_getch();
	_getch();
	return;
}

//-----------------------------------------------------------------------------
//Функція вилучає зі списку дані про книги авторів з назвою ,що починаю-
//ться на  латинську літеру J.
// SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
//Якщо список пустий ,то виводиться відповідне повідомлення
void  deleteBooksAuthorsNameBeginningletter_J(SbookItem **ppHeadListBooks)
{
	if (*ppHeadListBooks == NULL){
		cout << "No data about books\n";
		return;
	}
	char ch = 'J';
	SbookItem *pCurrent = *ppHeadListBooks;//Вказівник на поточний елемент
	SbookItem *pPreviev = nullptr;//Вказівник на попередній елемент
	SbookItem *pNext = nullptr;//Вказівник на наступний елемент
	while (pCurrent){
		if (pCurrent->Book.sAuthor[0] == ch){
			if (pPreviev != NULL) {
				pNext = pCurrent->pNextBook;
				pPreviev->pNextBook = pNext;
				delete pCurrent;
				pCurrent = pNext;
			}
			else {
				*ppHeadListBooks = pCurrent->pNextBook;
				delete pCurrent;
				pCurrent = *ppHeadListBooks;
			}	
		}
		else {
			pPreviev = pCurrent;
			pCurrent = pCurrent->pNextBook;
		}
	}
	printBooksData(*ppHeadListBooks);
	return;
}

//---------------------------------------------------------------------------------------------
//Функція копіює елементи однозв'язного списку у масив
// SbookItem *pHeadListBooks - вказівник на початок однозв'язного списку
// SbookData *arrayBook - масив,у який копіюються елементи однозв'язного списку
// const unsigned uBookCnt - кількість книжок в однозв'язному списку
void copyListToArray(SbookData *arrayBook, SbookItem *HeadListBooks, const unsigned uBookCnt)
{
	SbookItem *pCurrent = HeadListBooks;
	for (int i = 0; i < uBookCnt; ++i)
	{
		arrayBook[i] = pCurrent->Book;
		pCurrent = pCurrent->pNextBook;
	}
	return;
}
//------------------------------------------------------------------------------------------
//Функція міняє містями адреси SbookData *left з SbookData *right
inline void Swap(SbookData *left, SbookData *right)
{
	SbookData temp = *left;
	*left = *right;
	*right = temp; 
}
//Функція перевіряє рядок на допустимі символи і чи він не є пустим
//повертає false якщо рядок є пустим або в рядку є недопустимі символи -
//не латинська літера ,інакше повертає false
//Параметри :
//char *str -  вказівник на рядок 
bool checkStrAuthor(char *str)
{
	const int iSizeStr = strlen(str);
	bool isCorrect = true;
	for (int i = 0; i < iSizeStr; ++i) {
		if (!isalpha(str[i]) && !isspace(str[i]))
			isCorrect = false;
	}
	if(!isupper(*str))
		isCorrect = false;
	return isCorrect;
}

bool checkStrTitleBook(char *str)
{
	const int iSizeStr = strlen(str);
	bool isCorrect = true;
	for (int i = 0; i < iSizeStr; ++i) {
		if (!isalnum(str[i]) && !isspace(str[i]))// && 
			/*(str[i] != '.' || str[i] != ',' || 
				str[i] != '!' || str[i] != '-' || 
				str[i] != '?' || str[i] != ':' || str[i] != '`'))*/
			isCorrect = false;
	}
	if (!isupper(*str) && !isdigit(*str))
		isCorrect = false;
	return isCorrect;
}
bool checkStrIntVal(char *str)
{
	const int iSizeStr = strlen(str);
	int countDigits = 0;
	bool isCorrect = true;
	if(isdigit(*str) == 0)
		isCorrect = false;
	for (int i = 0; i < iSizeStr - 1; ++i) {
		if (!isdigit(str[i]))
			isCorrect = false;
		//if (isdigit(str[i])) {
		//	++countDigits;

		//}
	}
	return isCorrect;
}
bool checkStrFloatVal(char *str)
{
	bool isCorrect = true;
	int iCntPunctum = 0;
	const int iSizeStr = strlen(str);
	if (isdigit(str[0]) == 0)
		isCorrect = false;
	for (int i = 0; i < iSizeStr; ++i) {
		if (str[i] == '.' && i != 0)
			++iCntPunctum;
		if (isdigit(str[i]) == 0 && (iCntPunctum > 1 && str[i] == '.'))
			isCorrect = false;
	}
	//if(!isdigit(str[iSizeStr - 1]))
	//	isCorrect = false;
	return isCorrect;
}
bool checkSpace(char *str)
{
	bool isCorrect = true;
	const int iSizeStr = strlen(str);
	//cout << iSizeStr << endl;
	for (int i = 0; i < iSizeStr; ++i) {
		if (!isspace(str[i]))
			isCorrect = false;
	}
	return isCorrect;
}
inline void message()
{
	cout << "Fill in correctly !\n";
	cout << "Press any key to end program\n";
	_getch();
	exit(1);
}
inline void clearBuff()
{
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
}

