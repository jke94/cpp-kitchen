#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>

// TODO: GET more info.
// https://curl.se/libcurl/c/curl_easy_getinfo.html

class CurlMetric
{
private:
    CURL *curl_;
    CURLcode res_;

    std::string url_;
    std::string protocol_;


public:
    std::string response_;
    char *effective_url_ = NULL;
    char *effective_method_ = NULL;
    char *connection_type_ = NULL;
    char *local_ip_ = NULL;
    long local_port_;
    char *primary_ip_ = NULL;
    long primary_port_ = 0;
    long http_version_ = 0;
    long info_file_time_ = 0;
    char *schema_ = NULL;
    long response_code_ = 0;
    long http_connection_code_ = 0;
    long header_size_ = 0;         // Number of bytes of all headers received
    double connection_time_ = 0.0; // Seconds
    double connect_ = 0.0;
    double pre_transfer_ = 0.0;
    double start_transfer_ = 0.0;
    double total_time_ = 0.0; // Seconds
    double namelookup_ = 0.0; // Total time in seconds from the start until the name resolving was completed
    curl_off_t dl_;           // The total amount of bytes that were downloaded
    curl_off_t speed_;

    // static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
    CurlMetric();
    CurlMetric(std::string url, std::string protocol);
    ~CurlMetric();
    bool launch_request();
    std::string to_string();
};

static size_t CurlMetric::write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    static_cast<std::string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

CurlMetric::CurlMetric(std::string url, std::string protocol)
{
    url_ = url;
    protocol_ = protocol;
}

CurlMetric::CurlMetric()
{

}

bool CurlMetric::launch_request()
{
    curl_ = curl_easy_init();
    
    if(!curl_)
    {
        return false;
    }

    curl_easy_setopt(curl_, CURLOPT_URL, url_);
    curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl_, CURLOPT_DEFAULT_PROTOCOL, protocol_);
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, response_);

    res_ = curl_easy_perform(curl_);

    curl_easy_getinfo(curl_, CURLINFO_EFFECTIVE_URL, &effective_url_);
    curl_easy_getinfo(curl_, CURLINFO_EFFECTIVE_METHOD, &effective_method_);
    curl_easy_getinfo(curl_, CURLINFO_CONTENT_TYPE, &connection_type_);
    curl_easy_getinfo(curl_, CURLINFO_LOCAL_IP, &local_ip_);
    curl_easy_getinfo(curl_, CURLINFO_LOCAL_PORT, &local_port_);
    curl_easy_getinfo(curl_, CURLINFO_PRIMARY_IP, &primary_ip_);
    curl_easy_getinfo(curl_, CURLINFO_PRIMARY_PORT, &primary_port_);
    curl_easy_getinfo(curl_, CURLINFO_HTTP_VERSION, &http_version_);
    curl_easy_getinfo(curl_, CURLINFO_FILETIME_T, &info_file_time_);
    curl_easy_getinfo(curl_, CURLINFO_SCHEME, &schema_);
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code_);
    curl_easy_getinfo(curl_, CURLINFO_HTTP_CONNECTCODE, &http_connection_code_);
    curl_easy_getinfo(curl_, CURLINFO_HEADER_SIZE, &header_size_);
    curl_easy_getinfo(curl_, CURLINFO_CONNECT_TIME, &connection_time_);
    curl_easy_getinfo(curl_, CURLINFO_APPCONNECT_TIME, &connect_);
    curl_easy_getinfo(curl_, CURLINFO_PRETRANSFER_TIME, &pre_transfer_);
    curl_easy_getinfo(curl_, CURLINFO_STARTTRANSFER_TIME, &start_transfer_);
    curl_easy_getinfo(curl_, CURLINFO_TOTAL_TIME, &total_time_);
    curl_easy_getinfo(curl_, CURLINFO_NAMELOOKUP_TIME, &namelookup_);
    curl_easy_getinfo(curl_, CURLINFO_SIZE_DOWNLOAD_T, &dl_);
    curl_easy_getinfo(curl_, CURLINFO_SPEED_DOWNLOAD_T, &speed_);

    curl_easy_cleanup(curl_);

    std::cout << res_ << std::endl;

    return true;
}

CurlMetric::~CurlMetric()
{
    // curl_easy_cleanup(curl_);
}



std::string CurlMetric::to_string()
{
    std::stringstream ss;

    ss << "curl_version():       " << std::string(curl_version()) << "\n";
    ss << "response_ (size):     " << response_.size() << "\n";
    ss << "effective_url:        " << std::string(effective_url_) << "\n";
    ss << "effective_method:     " << std::string(effective_method_) << "\n";
    ss << "connection_type:      " << std::string(connection_type_) << "\n";
    ss << "local_ip:             " << std::string(local_ip_) << "\n";
    ss << "local_port:           " << local_port_ << "\n";
    ss << "primary_ip:           " << std::string(primary_ip_) << "\n";
    ss << "primary_port:         " << primary_port_ << "\n";
    ss << "http_version:         " << http_version_ << "\n";
    ss << "info_file_time:       " << info_file_time_ << "\n";
    ss << "schema:               " << std::string(schema_) << "\n";
    ss << "response_code:        " << response_code_ << "\n";
    ss << "http_connection_code: " << http_connection_code_ << "\n";
    ss << "header_size:          " << header_size_<< "\n";
    ss << "total_time:           " << total_time_ << "\n";
    ss << "connection_time:      " << connection_time_ << "\n";
    ss << "pre_transfer:         " << pre_transfer_ << "\n";
    ss << "start_transfer:       " << start_transfer_ << "\n";
    ss << "connect:              " << connect_ << "\n";
    ss << "namelookup:           " << namelookup_ << "\n";
    ss << "dl:                   " << std::to_string(dl_) << "\n";
    ss << "speed:                " << std::to_string(speed_) << "\n";

    std::string str = ss.str();

    return str;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    static_cast<std::string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

int main(void)
{
    // CURL *curl;
    // CURLcode res;

    // curl = curl_easy_init();
    
    // if (curl)
    // {
    //     CurlMetric *curl_metric = new CurlMetric();



    //     // curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/users");
    //     curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/comments");
    //     curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    //     curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    //     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    //     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_metric->response_);

    //     res = curl_easy_perform(curl);

    //     curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &curl_metric->effective_url_);
    //     curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_METHOD, &curl_metric->effective_method_);
    //     curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &curl_metric->connection_type_);
    //     curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &curl_metric->local_ip_);
    //     curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &curl_metric->local_port_);
    //     curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &curl_metric->primary_ip_);
    //     curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &curl_metric->primary_port_);
    //     curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &curl_metric->http_version_);
    //     curl_easy_getinfo(curl, CURLINFO_FILETIME_T, &curl_metric->info_file_time_);
    //     curl_easy_getinfo(curl, CURLINFO_SCHEME, &curl_metric->schema_);
    //     curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &curl_metric->response_code_);
    //     curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &curl_metric->http_connection_code_);
    //     curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &curl_metric->header_size_);
    //     curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &curl_metric->connection_time_);
    //     curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &curl_metric->connect_);
    //     curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &curl_metric->pre_transfer_);
    //     curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &curl_metric->start_transfer_);
    //     curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curl_metric->total_time_);
    //     curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &curl_metric->namelookup_);
    //     curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &curl_metric->dl_);
    //     curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &curl_metric->speed_);

    //     curl_easy_cleanup(curl);

    //     std::cout << curl_metric->to_string() << std::endl;
    //     std::cout << curl_metric->response_ << std::endl;

    //     delete curl_metric;
    // }

    CurlMetric *metric = new CurlMetric(
        "https://jsonplaceholder.typicode.com/comments",
        "https"
    );

    std::cout << metric->launch_request() << std::endl;
    // std::cout << metric->to_string() << std::endl;
    std::cout << metric->speed_ << std::endl;

    delete metric;

    return 0;
}