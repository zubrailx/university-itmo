## Structures

## Sections
* DatabaseMeta
* DatabaseSection
* TableSection

## Description

### Base

#### BaseSection
|Type    |Value|Description|
|--------|-----|-----------|
|uint8_t |type |           |
|uint32_t|size |           |

#### SOPointer
|Type     |Value  |Description    |
|---------|-------|---------------|
|fileoff_t|address|section address|
|sectoff_t|offset |               |

Pointer to string that is located in section (current or another).

---
### DatabaseMeta
|Type     |Value       |Description|
|---------|------------|-----------|
|bool     |is_corrupted|           |
|fileoff_t|ds_first    |           |
|fileoff_t|ds_last     |           |
|fileoff_t|pos_empty   |           |

Section to store metadata for database.

---

### DatabaseSection
|Type    |Value |Description|
|--------|------|-----------|
|DSHeader|header|           |
|char[]  |body  |           |

#### DSHeader
|Type       |Value       |Description      |
|-----------|------------|-----------------|
|BaseSection|base_section|                 |
|fileoff_t  |next        |                 |
|fileoff_t  |previous    |                 |
|bodyoff_t  |index_last  |after last index |
|bodyoff_t  |typle_start |first typle entry|


#### DSIndex
|Type     |Value     |Description|
|---------|----------|-----------|
|bodyoff_t|start     |           |
|bodyoff_t|end       |after last |
|bool     |is_cleared|           |


#### DSTyple
|Type     |Value |Description|
|---------|------|-----------|
|DSTHeader|header|           |

Next goes columns.

#### DSTHeader
|Type       |Value     |Description                                       |
|-----------|----------|--------------------------------------------------|
|fileoff_t  |fileoff   |                                                  |
|bool       |is_inline |                                                  |
|SOPointer[]|str_or_ptr|string or pointer to the string in another section|

#### DSTColumn
|Type           |Value     |Description                                       |
|---------------|----------|--------------------------------------------------|
|int8_t         |type      |                                                  |
|DSTColumnLimits|limits    |                                                  |
|bool           |is_inline |                                                  |
|SOPointer[]    |str_or_ptr|string or pointer to the string in another section|

#### DSTypleIn
|Type      |Value |Description|
|----------|------|-----------|
|DSTIHeader|header|           |

Inlined typle in memory for fast data access. Next goes columns

#### DSTIHeader
|Type     |Value  |Description|
|---------|-------|-----------|
|fileoff_t|fileoff|           |
|char[]   |name   |           |

#### DSTIColumn
|Type           |Value |Description|
|---------------|------|-----------|
|int8_t         |type  |           |
|DSTColumnLimits|limits|           |
|char[]         |name  |           |
