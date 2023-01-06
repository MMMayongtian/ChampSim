#define GHB_SIZE 256
#define IT_SIZE 256
#define UNKNOWN 256

#define look_head 1
#define degree 20

typedef struct IT_ENTRY IT_ENTRY;
typedef struct GHB_ENTRY GHB_ENTRY;

struct IT_ENTRY
{
    GHB_ENTRY *GHB_ENTRY_ptr = NULL;
};

struct GHB_ENTRY
{
    uint64_t addr;
    GHB_ENTRY *prev = NULL;
};

class GHB
{
    struct IT_ENTRY IT[IT_SIZE];
    struct GHB_ENTRY GHB[GHB_SIZE];
    unsigned int ghb_index = 0;
    uint64_t stride;
public:
    void setEntry(uint64_t addr, uint64_t ip){
        //设置GHB_ENTRY
        GHB[ghb_index].prev = IT[ip % IT_SIZE].GHB_ENTRY_ptr;
        GHB[ghb_index].addr = addr >> LOG2_BLOCK_SIZE;
        //更新IT_ENTRY
        IT[ip % IT_SIZE].GHB_ENTRY_ptr = &(GHB[ghb_index]);
    }

    bool missCheck(){
        //检测3次Miss
        GHB_ENTRY * prev = GHB[ghb_index].prev;
        if(prev!=NULL&&prev->prev!=NULL){
            return true;
        }
        return false;
    }
    bool strideCheck(){
        uint64_t stride1 = GHB[ghb_index].addr - GHB[ghb_index].prev->addr;
        uint64_t stride2 = GHB[ghb_index].prev->addr - GHB[ghb_index].prev->prev->addr;
        if(stride1==stride2){
            stride = stride1;
            return true;
        }
        return false;
    }
    uint64_t getStride(){
        return stride;
    }
    void updateIndex(){
        //更新index
        ghb_index = (ghb_index + 1) % GHB_SIZE;
    }
};
