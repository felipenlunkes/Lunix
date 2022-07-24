#define EDOM 1
#define ERANGE 2

#define errno (*(_errno()))

int *_errno(void);
