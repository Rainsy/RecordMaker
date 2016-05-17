/************************
 *
 * Written by jeon
 * rainsy02@gmail.com
 *
 ************************/

#define REPEAT		3
#define INDEX_SIZE	128
#define RECORD_SIZE	10000

#define UNIQUE  0
#define DUPLI   1

#include <stdio.h>

// Data structure. used for priority in time or space.
// ex) indexSize := recordSize    -> priority in time
// ex) indexSize := 0             -> priority in space
// ex) indexSize := recordSize/2  -> priortty in time (or space) as 50%
struct Record {
    int *p_recordMap;
    int i_recordSize;
    int i_indexSize;
    int i_index;
};

int initRecord(struct Record *p_record);

int checkDuplicated(struct Record *p_record, int input);

int main()
{
    int i;
    int randRecord;
    struct Record r1;

    initRecord(&r1);

    for(i=0; i<REPEAT ; )
    {
        randRecord = rand() % RECORD_SIZE;

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
    return 0;
}

/**
 @brief - Initialize Record structure.
 @param - p_record : pointer of Record structure.
 @return - 0 : Successful, 1 : Error
 */
int initRecord(struct Record *p_record)
{
    //TODO : realize when Record sturucter is used for priority in space

    if(p_record == NULL)
    {
        printf("ERR : p_record is null pointer.\n");
        return 1;
    }

    p_record->p_recordMap = (int*)malloc(sizeof(int) * RECORD_SIZE / INDEX_SIZE);
    p_record->i_recordSize = RECORD_SIZE;
    p_record->i_index = 0;

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
