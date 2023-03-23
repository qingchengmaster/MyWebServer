#include "http_conn.h"

int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;
//设置文件描述符非阻塞
void setnonblocking(int fd){
    int old_flag = fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flag);
}

//添加文件描述符到epoll中
extern void addfd(int epollfd, int fd, bool one_shot){
    epoll_event event;
    event.data.fd=fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    
    if(one_shot){
        event.events | EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    //设置文件描述符非阻塞
    setnonblocking(fd);
}

//从epoll中删除文件描述符
extern void removefd(int epollfd, int fd){
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

//修改文件描述符
extern void modfd(int epollfd, int fd,int ev){
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

//初始化连接
void http_conn::init(int sockfd, const sockaddr_in & addr){
    m_socket = sockfd;
    m_address = addr;

    //端口复用
    int reuse = 1;
    setsockopt(m_socket,SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    //添加到epoll对象中
    addfd(m_epollfd,m_socket,true);
    m_user_count++;//总用户数+1

}

//关闭连接
void http_conn::close_conn(){
    if(m_socket != -1){
        removefd(m_epollfd,m_socket);
        m_socket = -1;
        m_user_count--; // 关闭一个连接
    }
}

bool http_conn::read(){
    printf("一次性读完数据\n");
    return true;
}

bool http_conn::write(){
    printf("一次性写完数据\n");
    return true;
}

void http_conn::process(){
    //解析HTTP请求
    printf("解析\n");
    //生成响应
}