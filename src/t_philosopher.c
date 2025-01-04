#include "t_philosopher.h"

t_philosopher philosopher_new(u32 index, const t_config* cfg)
{
    return (t_philosopher){.thread = NULL, .index = index, .cfg = cfg};
}
