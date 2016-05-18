/************************
 *
 * Written by jeon
 * rainsy02@gmail.com
 *
 ************************/

// INDEX_SIZE : The number of index
// RANGE_SIZE : The range of value (MAX - MIN + 1) regardless of index

#define INDEX_SIZE	10000
#define RANGE_SIZE	10000

#define UNIQUE  0
#define DUPLI   1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Data structure. used for priority in time or space.
// ex) indexSize := recordSize    -> priority in time
// ex) indexSize := 0             -> priority in space
// ex) indexSize := recordSize/2  -> priortty in time (or space) as 50%
struct Record {
	// p_referMap : pointer of resource (reference map) which is used to store reference count.
	// i_indexSize : The number of index, The value 0(zero) means that this program operates as priority in space.
	// i_rangeSize : The range of value (MAX - MIN + 1) regardless of index
	// i_index : current index pointer indexing p_recordArr;
	// i_indexRefer : current index pointer indexing p_referMap
	// i_maxCount : the maximum counter which one row of record will be able to store. 
    int *p_referMap;
	int *p_recordArr;
    int i_indexSize;
    int i_rangeSize;
    int i_index;
	int i_IndexRefer;
	int i_arraySize;
	int i_maxCount;
};

int initRecord(struct Record *p_record, int _arraySize);

int checkDuplicated(struct Record *p_record, int input);

int addValue(struct Record *p_record);

void printRecord(const struct Record *p_record);

int main(int argc, char** argv)
{
    int i;
    int repeat, randRecord;
	struct Record r1 = { 0 };

	if (argc != 2)
	{
		printf("Usage : %s repeat_count\n", argv[0]);
		return 1;
	}

	repeat = atoi(argv[1]);

    initRecord(&r1, repeat);

	// Empty increasing statement,
	// This reason is to that duplication case is regarded as invalid
	// Instead, Increases in the case that is not duplicated.
    for(i=0; i<repeat  ; )
    {
        randRecord = rand() % RANGE_SIZE;

        if( checkDuplicated(&r1, randRecord) == DUPLI )
        {
             // randRecord is existed in r1 structure.
             // Continue loop without increase of i
             continue;
        }
        else
        {
            // TODO:Add record to r1 structure.

            // To repeat for loop, increase i
            i++;
        }
    }

	// Finally, Print record structure.
	printRecord(&r1);

    return 0;
}

/**
 @brief - Initialize Record structure.
 @param - p_record : pointer of Record structure.
 @return - 0 : Successful, 1 : Error
 */
int initRecord(struct Record *p_record, int _arraySize)
{
    //TODO : realize when Record sturucter is used for priority in space
	//TODO : calculate maxCount by using math.h and log()
	//TODO : branch when INDEX_SIZE is 0 (priority in space)

    if(p_record == NULL)
    {
        printf("ERR : p_record is null pointer.\n");
        return 1;
    }

	if (p_record->p_referMap != NULL)
	{
		// need to free()
		free(p_record->p_referMap);
		p_record->p_referMap = NULL;
	}

	if (p_record->p_recordArr != NULL)
	{
		// need to free()
		free(p_record->p_recordArr);
		p_record->p_recordArr = NULL;
	}

	p_record->i_rangeSize = RANGE_SIZE;
	p_record->i_indexSize = INDEX_SIZE;
	p_record->i_IndexRefer = 0;
	p_record->i_arraySize = _arraySize;
	p_record->p_recordArr = (int*)malloc(sizeof(int) * _arraySize);

	if (p_record->i_indexSize == 0)
	{
		// priority in space
		// none using addtional memory space
		
	}
	else    // this case needs to additional memory space
	{
		// calculate maxCount
		int exponent = (int)(log2(RANGE_SIZE) - log2(INDEX_SIZE) + 1);
		p_record->i_maxCount = (int)pow(2, exponent);

		// for alpha test version, exponent means row size of record.
		// This must is larger than size of integer.
		if (exponent > sizeof(int) * 8)  // 8 means a byte as bits
		{
			printf("Error, Index size is so small. Please set value larger than now.\n");
			printf("If you want mode to priority in space, Set Index size as 0(zero)\n");
			exit(1);
		}

		// The expression 'sizeof(int)' is in need of optimization to 'exponent'.
		// But considering trade off, if the coverage is not over 'exponent', 'sizeof(int)' is better,
		p_record->p_referMap = (int*)calloc(INDEX_SIZE, sizeof(int));
		p_record->i_index = 0;
	}
    return 0;
}

/**
 @brief - Check whatever input is existed in Record structure. 
 @param - p_record : pointer of Record Structure.
          input : input number
 @return - 0 : input is UNIQUE, 1 : input is DUPLIcated.
 */
int checkDuplicated(struct Record *p_record, int input)
{
    //TODO : realize body.
}

/**
 @brief - Add random value.
 @param - p_record : pointer of Record Structure.
 @return - 0 : Successful, 1 : Error.
*/
int addValue(struct Record *p_record)
{
	//TODO : realize body.
}

/**
@brief - Print all value within record structure.
@param - p_record : pointer of Record Structure.
@return - none.
*/
void printRecord(const struct Record * p_record)
{
	//TODO : realize body
}
