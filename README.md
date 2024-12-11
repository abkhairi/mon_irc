What is IRC (Internet Relay Chat)? :
    It allows clients to connect to servers and communicate in chat rooms (channels) or via private messages.

you must the irc : ./ircserv <port> <password>

first create socket() and the function return int file descriptor (fd)
int socket();



setsockopt :
    The setsockopt() function in network programming is used to configure options on a socket.
It allows fine-tuning of the behavior of sockets to meet specific requirements or improve performance.

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    sockfd: fd of socket
    level: يحدد المستوى الذي يتم تطبيق الخيار عليه. أمثلة: SOL_SOCKET: لتطبيق الخيارات العامة على السوكت.
IPPROTO_TCP: لتطبيق خيارات خاصة بـ TCP.
    optname: اسم الخيار الذي تريد تعديله (مثل SO_REUSEADDR أو SO_KEEPALIVE).
    optval: مؤشر للقيمة الجديدة التي تريد ضبطها.
    optlen: حجم المتغير المشار إليه بـ optval

int opt = 1; // تمكين الخيار
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
}
SOL_SOCKET: مستوى الخيارات العامة للسوكت.
SO_REUSEADDR: يتيح إعادة استخدام العنوان.
&opt: القيمة الجديدة للخيار (1 لتمكينه)


fcntl() :
     is a versatile tool to modify the behavior of file descriptors, particularly useful in scenarios like non-blocking I/O

Non-blocking behavior: I/O operations (read, write, etc.) will return immediately instead of waiting,
allowing the program to perform other tasks.

int fcntl(int fd, int cmd, ... /* arg */ );
    fd: The file descriptor you want to modify (e.g., a socket or an open file).
    cmd: The operation to perform on the file descriptor.
    arg: An optional argument, depending on the command (cmd).


التأثير: من بعد ما تدير هاد التغيير، أي عملية ديال القراءة أو الكتابة على السوكيت غادي ترجع فورا بدل ما تستنا وما غاديش تبلوك

poll()  : Using poll() (or equivalent) to handle multiple connections.
    the poll function in C is used to monitor multiple file descriptors to check 
if they are ready for input/output operations (e.g., reading, writing). The code you provided:

int ret = poll(fds, nfds, -1);
