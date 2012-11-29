#include <cstdlib>  
#include <cstring>  
#include <iostream>  
#include <boost/asio.hpp>  
#include <boost/bind.hpp>  

using boost::asio::ip::tcp;  
using namespace std;  

static int id = 1;  
const char message[] = "GET /info.txt HTTP/1.0\r\n\r\n";  
class echo_session  
{  
    public:  
        echo_session(boost::asio::io_service& io_service)   
            : socket_(io_service)  
        {  
            id_ = id;  
            ++id;  
        }  
        void start(const std::string& ip, const std::string& port)  
        {  
            //解析主机地址  
            tcp::resolver resolver(socket_.get_io_service());  
            tcp::resolver::query query(tcp::v4(), ip, port);  
            tcp::resolver::iterator iterator = resolver.resolve(query);  
            //异步连接  
            socket_.async_connect(*iterator, boost::bind(&echo_session::handle_connect, this, boost::asio::placeholders::error));  
        }  
    private:  
        void handle_connect(const boost::system::error_code& error)  
        {  
            if (!error)  
            {  
                //连接成功，发送message中的数据  
                boost::asio::async_write(socket_,   
                        boost::asio::buffer(message, sizeof(message)),  
                        boost::bind(&echo_session::handle_write, this,  
                            boost::asio::placeholders::error));  
            }  
            else  
                cout << "error:"<<error << endl;  
        }  
        void handle_write(const boost::system::error_code& error)  
        {  
            if (!error)  
            {  
                //写入完毕，接收服务器回射的消息  
                boost::asio::async_read(socket_, boost::asio::buffer(buffer_),  
                        boost::bind(&echo_session::handle_read, this,  
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));  
            }  
            else  
                cout << "error:"<< error << endl;  
        }  
        void handle_read(const boost::system::error_code& error, size_t bytes_transferred)  
        {  
            if (!error)  
            {  
                //读取完毕，在终端显示  
                cout << id_ << ":receive:" << bytes_transferred << "@@@@@" << buffer_.data() << "\n\n";  
                //周而复始...  
                //handle_connect(error);  
            }  
            else  
                cout << "error:" <<error.message() << endl;  
        }  
        int id_;  
        tcp::socket socket_;
        boost::array<char, 100000> buffer_;  
};  
