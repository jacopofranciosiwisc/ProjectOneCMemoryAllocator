#include "mem.h"
extern BLOCK_HEADER* first_header;

/* Helper function to get the alloc_bit
 * @param *ptr pointer to a header in memory
 * @return 0 if memory block is free, 1 if not
 */
int check_Alloc_Bit(BLOCK_HEADER* ptr){
    return ptr ->size_alloc & 1;
}

/* Helper function to get the block_size
 * @param *ptr pointer to a hader in memory
 * @return size of block 
 */
int get_Block_Size(BLOCK_HEADER* ptr){
    return ptr->size_alloc & 0xFFFFFFFE;
}

/* Helper function to set the allocated bit. In this case, 1 because it's allocated
 * @param *ptr pointer to a header in memory
 * @return nothing
 */
void set_Alloc_Bit(BLOCK_HEADER* ptr){
    if (ptr -> size_alloc % 2 == 0) ptr -> size_alloc++;
}

/* Helper function to set the free bit. In this case, 0 because it's free
 * @param *ptr pointer to a header in memory
 * @return nothing
 */
void set_Free_Bit(BLOCK_HEADER* ptr){
    if (ptr -> size_alloc % 2 == 1) ptr -> size_alloc--;
}

// helpers for getting payload size, user_pointer, padding

/* Helper function to get the payload size
 * @param *ptr pointer to a header in memory
 * @return size of block 
 */
int get_Payload(BLOCK_HEADER* ptr){
    return ptr->payload;
}

/* Helper function to get the padding
 * @param *ptr pointer to a header in memory
 * @return size of padding
 */
int get_Padding(int p_size, int pad){
    return p_size - 8 - pad;
}

/* Helper function to get the next header
 * @param *ptr pointer to a header in memory
 * @return size of block 
 */
void* get_N_Header(BLOCK_HEADER* ptr){
    int size = get_Block_Size(ptr);
    return (BLOCK_HEADER*)((unsigned long)ptr + size);

}

/* Helper function to get the user pointer
 * @param *ptr pointer to a header in memory
 * @return size of block 
 */
void* get_User_Pointer(BLOCK_HEADER* ptr){
    return (void *)((unsigned long)ptr + 8);
}

// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size){
    // find a free block that's big enough
    BLOCK_HEADER *curr = first_header;

    int block_size = get_Block_Size(curr);
    
    // iterating through the blocks
    while(1){
        
        // if we can't find a block (at the last header), return null
        if (curr -> size_alloc == 1){
            return NULL;
        }

        if ((check_Alloc_Bit(curr) == 0) && (block_size - size - 8 >= 0)){
            break;
        }


        //move onto the next block
        curr = get_N_Header(curr);

        /*
        // have a condition, check if current block if free, and check if the payload size available is >=
        curr = (BLOCK_HEADER *)((unsigned long)curr + block_size);
        alloc = check_Alloc_Bit(curr);
        block_size = get_Block_Size(curr);
        payload = get_Payload(curr);
        */
        
    }

    // check how much space we have until we have a div. by 16 number
    int counter = 0;
    int totPad = 8 + size;
    while ((totPad + counter) % 16 != 0) {
        counter++;
    }

    // now that we fixed the padding, we can allocate the block
    set_Alloc_Bit(curr);

    // split if necessary (if padding size is greater than or equal to 16 split the block)
    //make a helper, input a pointer to find its helper

    // set the payload to the new size, update the block size, and get the new padding
    curr -> payload = size;
    int new_block_size = get_Block_Size(curr);
    int pad = get_Padding(new_block_size, size);

    if (pad >= 16 && pad - counter >= 16){
        BLOCK_HEADER *n;
        curr->size_alloc = size + check_Alloc_Bit(curr) + counter + 8;
        n = (BLOCK_HEADER*)((unsigned long) curr + get_Block_Size(curr));
        n -> size_alloc = pad - counter; n -> payload = pad - counter - 8;
    }else{
        curr -> size_alloc = check_Alloc_Bit(curr) + pad + size + 8;
    }
    
    return get_User_Pointer(curr);
}

// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr){
    // traverse the list and check all pointers to find the correct block 
    // if you reach the end of the list without finding it return -1
    BLOCK_HEADER *curr = first_header;

    // loop through the blocks
    while (1){

        // get the current size
        int currSize = curr -> size_alloc;

        if (get_User_Pointer(curr) == ptr) break; // break if the user pointer is equal to ptr, so exit the looping
        if (currSize == 1) return -1; // invalid case
        curr = get_N_Header(curr); // new header

    }
    // free the block 
    set_Free_Bit(curr);
    // add the payload and padding
    curr -> payload += get_Padding(get_Block_Size(curr), get_Payload(curr));

    // coalesce adjacent free blocks

    for (int i = 0; i < 2; i++){
        curr = first_header;

        while (1){
            if (check_Alloc_Bit(curr) != 1){
                if (check_Alloc_Bit(get_N_Header(curr)) == 0) break;
            }
            if (curr -> size_alloc == 1) return 0; // success

            // go to the next header
            curr = get_N_Header(curr);
        }

        int addr_dis = ((unsigned long) get_N_Header(get_N_Header(curr)) - (unsigned long) curr);
        int new_payload = addr_dis - 8;
        curr -> size_alloc = addr_dis; curr -> payload = new_payload;
        curr = first_header;
    }

    return 0;
}

