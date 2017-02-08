#include"in_out.h"

static int read_cnt;
static char * read_ptr;
static char read_buf[MAXLINE];

static ssize_t my_read(int fd, char * ptr)
{
	if(read_cnt <= 0 ) {
		again:
		  if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			  if (errno == EINTR)
			  		goto again;
			  return (-1);
	  	  } else if (read_cnt == 0)
	  		  return (0);
	  	read_ptr = read_buf;
 	}
 	read_cnt--;
 	
 	/* copy the character, move pointer to next character */
 	*ptr = *read_ptr++;
 	
 	/* return value of 1 byte read */
 	return (1);
}
			  
/********************************************************************
* readn()
*
* Description:
* Used to read a specified number of bytes from a file descriptor.
*
* Arguments:
*    fd   - the file descriptor to read from
*    vptr - the buffer to put read bytes into
*    n    - the number of bytes to read
*
* Return values:
*    On success: returns number of bytes successfully read
*    On failure: returns -1
*
********************************************************************/

ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t nleft;  /* # bytes left to read */
	ssize_t nread; /* # bytes alread read */
	char * ptr;
	
	ptr = vptr;
	nleft = n;
	while (nleft > 0 ) {
		if ( ( nread = read(fd, ptr, nleft)) < 0 ){
			if ( errno == EINTR )
				nread = 0;
			else
				return (-1);
		}else if (nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}

/********************************************************************
* writen()
*
* Description:
* Used to write a specified number of bytes to a file descriptor.
*
* Arguments:
*    fd   - the file descriptor to write to
*    vptr - the buffer to write into
*    n    - the number of bytes to write
*
* Return values:
*    On success: returns number of bytes successfully written
*    On failure: returns -1
*
********************************************************************/

ssize_t writen(int fd, const void * vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char * ptr;
	
	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if ( ( nwritten = write(fd, ptr, nleft)) <= 0 ) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return (-1);
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

/********************************************************************
* readline() (fast version of readline)
*
* Description:
* Reads up to (maxlen - 1) bytes into a read buffer specified by
* vptr.
*
* Arguments:
*    fd     - the file descriptor to read from
*    vptr   - the buffer read the data into
*    maxlen - the maximum number of bytes to be read
*
* Return values:
*    On success:  returns 0 or number of bytes read/copied
*    On failure:  returns -1
*
********************************************************************/

ssize_t readline(int fd, void * vptr, size_t maxlen)
{
	ssize_t n, rc;
	char    c, *ptr;
	
	ptr = vptr;
	for(n = 1; n < maxlen; n++) {
		if ( ( rc = my_read(fd,&c)) == 1 ) {
			*ptr++ = c;
			if(c == '\n')
				break;
		} else if (rc == 0) {
			*ptr = 0;
			return (n - 1);
		} else
			return (-1);
	}
	*ptr = 0;
	return (n);
}
