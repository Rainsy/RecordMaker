/************************
 *
 * Written by jeon
 * rainsy02@gmail.com
 *
 ************************/

// REFER_SIZE : The number of Reference index
// RECORD_SIZE : The range of record value (MAX - MIN + 1) regardless of index

#define REFER_SIZE	100
#define RECORD_SIZE	100

#define UNIQUE  0
#define DUPLI   1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Data structure. used for priority in time or space.
// ex) referSize := recordSize    -> priority in time
// ex) referSize := 0             -> priority in space
// ex) referSize := recordSize/2  -> priortty in time (or space) as 50%
struct Record {
	// p_referMap : pointer of resource (reference map) which is used to store reference count.
	// i_referSize : The number of Reference index, The value 0(zero) means that this program operates as priority in space.
	// i_arraySize : The number of repeat count which equal to array size as twosise
	// i_recordSize : The range of record value (MAX - MIN + 1) regardless of index
	// i_arrayIndex : Current index pointer indexing p_recordArr;
	// i_maxCount : The maximum counter which one row of record will be able to store. 
	// i_recordSize : The range of record value (MAX - MIN + 1) regardless of index
    int *p_referMap;
	int *p_recordArr;
    int i_referSize;
	int i_arraySize;
    int i_arrayIndex;
	int i_maxCount;
    int i_recordSize;
};

// for debug
// total_dupliCount is that how many duplication is occured
// total_checkReferenceCount is that how many check and calculate Reference
int total_dupliCount;
int total_checkReferenceCount;
int total_checkRecordCount;

int initRecord(struct Record *p_record, int _arraySize);
int checkDuplicated(struct Record *p_record, int input);
int addValue(struct Record *p_record, int input);
void printRecord(const struct Record *p_record);
void deleteRecord(struct Record *p_record);

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
        randRecord = rand() % RECORD_SIZE;

        if( checkDuplicated(&r1, randRecord) == DUPLI )
        {
            // randRecord is existed in r1 structure.
            // Continue loop without increase of i
			total_dupliCount++;
            continue;
        }
        else
        {
            // TODO:Add record to r1 structure.
			addValue(&r1, randRecord);
            // To repeat for loop, increase i
            i++;
        }
    }

	// Finally, Print record structure.
	printRecord(&r1);

	// Delete resource
	deleteRecord(&r1);

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
	//TODO : branch when REFER_SIZE is 0 (priority in space)

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

	p_record->i_recordSize = RECORD_SIZE;
	p_record->i_referSize = REFER_SIZE;
	// p_recordArr has pointed twosise of arraySize
	// ex) NTxxxxx yy
	// First of reference is 'xxxxx' and Second is 'yy'
	p_record->i_arraySize = _arraySize * 2;
	p_record->p_recordArr = (int*)malloc(sizeof(int) * _arraySize * 2);

	if (p_record->i_referSize == 0)
	{
		// priority in space
		// none using addtional memory space
		p_record->i_referSize = 0;
	}
	else    // this case needs to additional memory space
	{
		int exponent;
	
		exponent = log2(RECORD_SIZE) - log2(REFER_SIZE) + 1;

		if (RECORD_SIZE == REFER_SIZE)
			//in case, exponent is must be 0(zero), exception handling
			p_record->i_maxCount = 1;
		else
			//in case, at least priorty space
			//assign max Count in relation to exponent
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
		p_record->p_referMap = (int*)calloc(REFER_SIZE, sizeof(int));
		p_record->i_arrayIndex = 0;
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

	int indexRecord;
	int dupliCount;
	int quotient;
	int record;

	if (p_record->i_referSize == 0)
	{
		// priority in space
		for (indexRecord = 0;
			indexRecord < p_record->i_arrayIndex;
			indexRecord += 2)
		{
			// for debug
			total_checkRecordCount++;

			// access record array
			record = p_record->p_recordArr[indexRecord];

			// in case finding duplicated record
			// stop finding process, retrun DUPLI
			if (record == input)
				return DUPLI;
		}

		// in case out of the above for loop
		// that is regard that there is no duplicated
		// return UNIQUE

		return UNIQUE;

	}
	else
	{
		// for debug
		total_checkRecordCount++;

		// at least priority in time
		quotient = input / p_record->i_maxCount;

		for (indexRecord = 0, dupliCount = 0;
			indexRecord < p_record->i_arraySize;
			indexRecord += 2)
		{
			// in case finding successful as Reference count
			// stop finding process, return UNIQUE
			if (p_record->p_referMap[quotient] == dupliCount)
				return UNIQUE;

			// access record array
			record = p_record->p_recordArr[indexRecord];

			// in case finding duplicated record
			// stop finding process, retrun DUPLI
			if (record == input)
				return DUPLI;

			// in case finding value including same quotient(index)
			// increasing dulicated count, continue loop
			if (record / p_record->i_maxCount == quotient) {
				total_checkReferenceCount++;
				dupliCount++;
			}
		}

		// in case out of the above for loop
		// that is regard to 'p_record->p_referMap[quotient] == dupliCount'
		// return UNIQUE

		// below is just for readability
		//if (p_record->p_referMap[quotient] == dupliCount)
			return UNIQUE;
	}

	// in case, error
	// be disable to access here if not debug mode
	printf("Error, checkDuplicated(), out of access instruction\n");
	exit(1);
}

/**
 @brief - Add random value.
 @param - p_record : pointer of Record Structure.
 @return - 0 : Successful, 1 : Error.
**/
int addValue(struct Record *p_record, int input)
{
	//TODO : realize body.
	//TODO : check whether p_arr is full or able to add
	//TODO : returnsucessful or not
	//TODO : increase count in which Reference.
	int index;
	int *p_arr;

	index = p_record->i_arrayIndex;
	p_arr = p_record->p_recordArr;

	if (index / 2 >= p_record->i_arraySize)
	{
		// Aleady array is full
		return 1;
	}

	p_arr[index] = input;
	p_arr[index + 1] = (rand() % 20) * 5;

	p_record->i_arrayIndex += 2;

	if (p_record->i_maxCount != 0) {
		// increase count in which Reference
		int quotient;
		quotient = input / p_record->i_maxCount;

		p_record->p_referMap[quotient] += 1;
	}
	return 0;

}

/**
@brief - Print all value within record structure.
@param - p_record : pointer of Record Structure.
@return - none.
*/
void printRecord(const struct Record * p_record)
{
	//TODO : realize body

	for (int i = 0; i < p_record->i_arraySize; i+=2)
	{
		printf("NT%05d %02d\n", p_record->p_recordArr[i], p_record->p_recordArr[i + 1]);
	}

	printf("\n");
	printf("Total duplicated case : %d\n", total_dupliCount);
	printf("Total check reference case : %d\n", total_checkReferenceCount);
	printf("Total check record case : %d\n", total_checkRecordCount);
}

/**
 @brief - Delete inside resource.
 @param - p_record : pointer of Record Structure.
 @return - none.
*/
void deleteRecord(struct Record * p_record)
{
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
	return;
}