/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 21:48:19
 * LastEditTime : 2022-05-31 13:48:41
 * Description  : Bit Map
 */

#ifndef _BIT_MAP_H_
#define _BIT_MAP_H_

#include "os_type.h"

/* WARNING:
 *  BITMAP IS NOT AVAILABLE BEFORE HEAP IS INITIALIZED
 *  SINCE BITMAP RELIES ON THE ALLOC FUNCTION OF HEAP
 */

class BitMap {
    private:
        uint8* map;
        int size;
        int freeSize;
        /**
         * Find the first available block in the map
         * 
         * @param index the first index to start the search
         * @return the index of first avaible resource
         */
        int firstAvailable(int start);
    public:
        BitMap(int length);
        /**
         * Fetch the amount of resources available
         * 
         * @return the amount of resources available
         */
        int availableResources();
        /**
         * Set the status of specified index
         * 
         * @param index the specified resource
         * @param status the status to be set
         * @return the status after setting
         */
        bool set(int index, bool status);
        /**
         * Allocate one resource from the bit map
         * 
         * @param size the amount of pages to be allocated
         * @return the index of the allocated resource
         */
        int alloc();
        /**
         * Free the specific resource
         * 
         * @param index the index of the resource to be freed
         * @return the actual amount of resources freed
         */
        int free(int index);
};

#endif