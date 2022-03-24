#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/***************************************
*name    : open_port
*功能描述: 打开串口
*入口参数: 串口号
*返 回 值: 成功返回文件描述符，失败返回负值
*作    者: 
*修改时间:
***************************************/

struct termios new_cfg, old_cfg;
int open_port(const char * com_port)
{
    int fd;

	if( com_port == NULL ){
        printf("the port name is null\n");
        return -1;
    }
    /*open port*/
    fd = open(com_port, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0){
        fd = open(com_port, O_RDWR | O_NOCTTY | O_NDELAY);
        if(fd < 0){
            perror("open serial port");
            return -1;
        }
    }
    printf("open %s OK!\n", com_port);
    if(fcntl(fd, F_SETFL,0) < 0){
        perror("fcntl F_SETFL");
    }

    if(isatty(fd) == 0){
        perror("isatty is not a terminal device");
    }
    return fd;
}

/******************************
*name    : set_port
*功能描述: 设置串口参数
*入口参数: fd 文件描述符, baud_rate 波特率, data_bits 数据位,
*          parity 奇偶校验, stop_bits 停止位
*			调用示例: set_port(fd, 115200, 8, 'N',1);
*返 回 值: 成功返回0，失败返回-1
*作    者: 
*修改: 
******************************/
int set_port(int fd, int baud_rate,
             int data_bits, char parity, int stop_bits)
{
    int speed_arry[]= {B2400, B4800, B9600, B19200, B38400,B57600, B115200};
    int speed[]={2400,4800,9600,19200,38400,57600,115200};
    int i = 0;

    /*save and test the serial port*/
    if(tcgetattr(fd, &old_cfg) < 0){
        perror("tcgetattr");
        return -1;
    }
	
	if(fcntl(fd,F_SETFL,0) < 0)//恢复为阻塞模式
	{
		perror("fcntl(CzjFd,F_SETFL,0)!");
	}

    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);     //配置为原来配置
    new_cfg.c_cflag &= ~ CSIZE;     //用数据位掩码清空数据位的设置

    /*set baud_rate*/
    for(i = sizeof(speed_arry) / sizeof(speed_arry[0]); i > 0; i--)
    {
        if(baud_rate == speed[i]){
           // cfsetispeed(&new_cfg,speed_arry[i]);
           // cfsetospeed(&new_cfg,speed_arry[i]);
            cfsetspeed(&new_cfg,speed_arry[i]);
        }
    }

    switch(data_bits)    /*设置数据位*/
    {
        case 7:
                new_cfg.c_cflag |= CS7;
                break;

        default:
        case 8:
                new_cfg.c_cflag |= CS8;
                break;
    }

    switch(parity)
    {
        default:
        case 'N':
        case 'n':
        {
            new_cfg.c_cflag &= ~PARENB;     //清除校验位
            new_cfg.c_iflag &= ~(ICRNL|INPCK|IXON|IXOFF);      //关闭奇偶校验  关闭软件流控
            
			break;
        }

        case 'o':
        case 'O':
        {
            new_cfg.c_cflag |= (PARODD | PARENB); //使用奇校验不是用偶校验
            new_cfg.c_iflag |= INPCK;
			break;
        }

        case 'e':
        case 'E':
        {
            new_cfg.c_cflag |= PARENB;
            new_cfg.c_cflag &= ~PARODD;     //使用偶校验
			new_cfg.c_iflag |= INPCK;
			break;
        }

        case 's':
        case 'S':
        {
            new_cfg.c_cflag &= ~PARENB;
            new_cfg.c_cflag &= ~CSTOPB;
			break;
        }
    }
	
    new_cfg.c_iflag &= ~(ICRNL| IXON | IXOFF  );      //关闭奇偶校验  关闭软件流控
	new_cfg.c_oflag &= ~OPOST;

    switch(stop_bits)
    {
        default:
        case 1:
        {
            new_cfg.c_cflag &= ~CSTOPB;
            new_cfg.c_cflag &= ~CRTSCTS;   //禁用硬件流控
            //new_cfg.c_cflag |= CRTSCTS;    //启用硬件流控
			break;
        }
        case 2:
        {
            new_cfg.c_cflag |= CSTOPB;
			break;
		}
    }

    /*set wait time*/
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN]  = 1;

    tcflush(fd, TCIFLUSH);   //处理未接收字符
	if((tcsetattr(fd, TCSANOW, &new_cfg)) < 0)
    {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int main()
{
	int len,i;
	int fd = open_port("/dev/ttymxc2");
	if ( fd < 0 )
	{
		perror("open port");
		return -1;
	}
	
	set_port(fd, 115200, 8, 'N',1);
	
	char readBuf[32] ={0};
	const char *pstr="hello world";
	write(fd, pstr, strlen(pstr)+1);
	
	len = read(fd, readBuf, sizeof(readBuf));
	readBuf[len] = '\0';
	printf("%s",readBuf);
	tcsetattr(fd,TCSANOW,&old_cfg);
	close(fd);
}

