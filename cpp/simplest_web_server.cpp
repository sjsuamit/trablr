#include <iostream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "mongoose.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static int s_sig_num = 0;
static const struct mg_str s_post_method = MG_MK_STR("POST");
static const struct mg_str s_get_method = MG_MK_STR("GET");
static const struct mg_str s_put_method = MG_MK_STR("PUT");
static const struct mg_str s_delete_method = MG_MK_STR("DELETE");

static const string host_name = "http://api.511.org";
static const string api_key = "876be52b-5ed7-46bf-bfe9-43443887b34a";
static const string format = "JSON";
static string API_RESPONSE_JSON;

// static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
//     ((std::string*)userp)->append((char*)contents, size * nmemb);
//     return size * nmemb;
// }

static size_t writeCallback(char* buff, size_t size, size_t nmemb) {
    for (size_t c = 0; c < size*nmemb; c++) {
        if (static_cast<int>(buff[c]) >= 0)
            API_RESPONSE_JSON.push_back(buff[c]);
    }
    return size*nmemb;
}

static void performCurl(string ApiUrl) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ApiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &API_RESPONSE_JSON);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
}

static void get511ApiTransitOperators(struct mg_connection *nc) {
    string api_prefix = "/transit/operators";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }

    assert(document.IsArray());
    for (SizeType i = 0; i < document.Size(); i++) {
        assert(document[SizeType(i)].IsObject());
        const Value& temp = document[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitLines(struct mg_connection *nc) {
    string api_prefix = "/transit/lines";
    string operator_id = "VTA";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&operator_id=" + operator_id
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsArray());
    for (SizeType i = 0; i < document.Size(); i++) {
        assert(document[SizeType(i)].IsObject());
        const Value& temp = document[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitStops(struct mg_connection *nc) {
    string api_prefix = "/transit/stops";
    string operator_id = "VTA";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&operator_id=" + operator_id
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsObject());
    assert(document.HasMember("Contents"));
    assert(document["Contents"].HasMember("dataObjects"));
    assert(document["Contents"]["dataObjects"].HasMember("ScheduledStopPoint"));
    const Value& stops = document["Contents"]["dataObjects"]["ScheduledStopPoint"];
    assert(stops.IsArray());
    for (SizeType i = 0; i < stops.Size(); i++) {
        assert(stops[SizeType(i)].IsObject());
        const Value& temp = stops[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitStopPlaces(struct mg_connection *nc) {
    string api_prefix = "/transit/stopPlaces";
    string operator_id = "VTA";
    string stop_id = "60824";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&operator_id=" + operator_id
                            + "&stop_id=" + stop_id
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsObject());
    assert(document.HasMember("Siri"));
    assert(document["Siri"].HasMember("ServiceDelivery"));
    assert(document["Siri"]["ServiceDelivery"].HasMember("DataObjectDelivery"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"].HasMember("dataObjects"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"].HasMember("SiteFrame"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"].HasMember("stopPlaces"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"].HasMember("StopPlace"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"].HasMember("Centroid"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"]["Centroid"].HasMember("Location"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"]["Centroid"]["Location"].HasMember("Longitude"));
    assert(document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"]["Centroid"]["Location"].HasMember("Latitude"));
    const Value& longitude = document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"]["Centroid"]["Location"]["Longitude"];
    const Value& latitude = document["Siri"]["ServiceDelivery"]["DataObjectDelivery"]["dataObjects"]["SiteFrame"]["stopPlaces"]["StopPlace"]["Centroid"]["Location"]["Latitude"];
    assert(longitude.IsString());
    assert(latitude.IsString());
    cout << "Longitude : " << longitude.GetString() << endl;
    cout << "Latitude : " << latitude.GetString() << endl;

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitStopMonitoring(struct mg_connection *nc) {
    string api_prefix = "/transit/StopMonitoring";
    string agency = "VTA";
    string stopCode = "60824";
    // string stopCode = "62744";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&agency=" + agency
                            + "&stopCode=" + stopCode
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsObject());
    assert(document.HasMember("ServiceDelivery"));
    assert(document["ServiceDelivery"].HasMember("StopMonitoringDelivery"));
    assert(document["ServiceDelivery"]["StopMonitoringDelivery"].HasMember("MonitoredStopVisit"));
    const Value& stopVisit = document["ServiceDelivery"]["StopMonitoringDelivery"]["MonitoredStopVisit"];
    assert(stopVisit.IsArray());
    for (SizeType i = 0; i < stopVisit.Size(); i++) {
        assert(stopVisit[SizeType(i)].IsObject());
        const Value& temp = stopVisit[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitVehicleMonitoring(struct mg_connection *nc) {
    string api_prefix = "/transit/VehicleMonitoring";
    string agency = "VTA";
    string vehicleID = "60824";
    // string vehicleID = "62744";
    // string vehicleID = "60519";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&agency=" + agency
                            // + "&vehicleID=" + vehicleID
                            + "&format=" + format;

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsObject());
    assert(document.HasMember("Siri"));
    assert(document["Siri"].HasMember("ServiceDelivery"));
    assert(document["Siri"]["ServiceDelivery"].HasMember("VehicleMonitoringDelivery"));
    assert(document["Siri"]["ServiceDelivery"]["VehicleMonitoringDelivery"].HasMember("VehicleActivity"));
    const Value& vehicleActivity = document["Siri"]["ServiceDelivery"]["VehicleMonitoringDelivery"]["VehicleActivity"];
    assert(vehicleActivity.IsArray());
    for (SizeType i = 0; i < vehicleActivity.Size(); i++) {
        assert(vehicleActivity[SizeType(i)].IsObject());
        const Value& temp = vehicleActivity[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void get511ApiTransitTimetable(struct mg_connection *nc, struct http_message *hm) {
    string api_prefix = "/transit/timetable";
    string operator_id = "VTA";
    string line_id = "60";
    string API_REQUEST_URL = host_name + api_prefix
                            + "?api_key=" + api_key
                            + "&operator_id=" + operator_id
                            + "&line_id=" + line_id
                            + "&format=" + format;

    /* Get form variables */
    char bus1[100], start1[100], end1[100];
    mg_get_http_var(&hm->body, "bus1", bus1, sizeof(bus1));
    mg_get_http_var(&hm->body, "start1", start1, sizeof(start1));
    mg_get_http_var(&hm->body, "end1", end1, sizeof(end1));

    API_RESPONSE_JSON.clear();
    performCurl(API_REQUEST_URL);

    Document document;
    if (document.Parse<0>(API_RESPONSE_JSON.c_str()).HasParseError()) {
        fprintf(stderr, "JSON Parsing error\n");
    }
    assert(document.IsObject());
    assert(document.HasMember("Content"));
    assert(document["Content"].HasMember("TimetableFrame"));
    const Value& timetableFrame = document["Content"]["TimetableFrame"];
    assert(timetableFrame.IsArray());
    for (SizeType i = 0; i < timetableFrame.Size(); i++) {
        assert(timetableFrame[SizeType(i)].IsObject());
        const Value& temp = timetableFrame[SizeType(i)];
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        temp.Accept(writer);
        cout << buffer.GetString() << endl;
    }

    /* Compute the result and send it back as a JSON object */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_printf_http_chunk(nc, "{ \"result\": %d }", 99);
    mg_send_http_chunk(nc, "", 0);
}

static void loopTest(struct mg_connection *nc) {
    int x = 98;
    while (x < 110)
    {
        x++;
        sleep(2);
        mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        mg_printf_http_chunk(nc, "{ \"result\": %d }", x);
        mg_send_http_chunk(nc, "", 0);
    }
}

static void handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
    char n1[100], n2[100];
    double result;

    /* Get form variables */
    mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
    mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));

    printf("n1=%s n2=%s\n", n1, n2);

    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

    /* Compute the result and send it back as a JSON object */
    result = strtod(n1, NULL) + strtod(n2, NULL);
    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void signal_handler(int sig_num) {
    signal(sig_num, signal_handler);
    s_sig_num = sig_num;
}

static int has_prefix(const struct mg_str *uri, const struct mg_str *prefix) {
    return uri->len > prefix->len && memcmp(uri->p, prefix->p, prefix->len) == 0;
}

static int is_equal(const struct mg_str *s1, const struct mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    static const struct mg_str api_prefix = MG_MK_STR("/api/v1");
    struct http_message *hm = (struct http_message *) ev_data;
    // struct mg_str key;

    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (has_prefix(&hm->uri, &api_prefix)) {
                // key.p = hm->uri.p + api_prefix.len;
                // key.len = hm->uri.len - api_prefix.len;
                if (mg_vcmp(&hm->uri, "/api/v1/route") == 0) {
                    get511ApiTransitTimetable(nc, hm);
                    cout << (string*)&hm->uri << endl;
                    // printf("%s\n", &hm->uri);
                }
                else if (is_equal(&hm->method, &s_get_method)) {
                    // get511ApiTransitLines(nc);
                    // get511ApiTransitStops(nc);
                    get511ApiTransitVehicleMonitoring(nc);
                    mg_printf(nc, "%s", "get_method\r\n");
                    printf("get_method\n");
                } 
                // else if (is_equal(&hm->method, &s_put_method)) {
                //     // db_op(nc, hm, &key, s_db_handle, API_OP_SET);
                //     mg_printf(nc, "%s", "put_method\r\n");
                //     printf("put_method\n");
                // } 
                // else if (is_equal(&hm->method, &s_delete_method)) {
                //     // db_op(nc, hm, &key, s_db_handle, API_OP_DEL);
                //     mg_printf(nc, "%s", "delete_method\r\n");
                //     printf("delete_method\n");
                // }
                else if (is_equal(&hm->method, &s_post_method)) {
                    // get511ApiTransitOperators(nc);
                    // get511ApiTransitStopPlaces(nc);
                    get511ApiTransitStopMonitoring(nc);
                    // get511ApiTransitTimetable(nc);
                    // loopTest(nc);
                    mg_printf(nc, "%s", "post_method\r\n");
                    printf("post_method\n");
                } else {
                    mg_printf(nc, "%s",
                    "HTTP/1.0 501 Not Implemented\r\n"
                    "Content-Length: 0\r\n\r\n");
                    printf("HTTP/1.0 501 Not Implemented\n");
                }
            } else {
                mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
                printf("Serve static content\n");
            }
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    int i;

    /* Open listening socket */
    mg_mgr_init(&mgr, NULL);
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = "/var/www/html";

    /* Parse command line arguments */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            s_http_server_opts.document_root = argv[++i];
        }
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* Run event loop until signal is received */
    printf("Starting RESTful server on port %s\n", s_http_port);
    while (s_sig_num == 0) {
        mg_mgr_poll(&mgr, 1000);
    }

    /* Cleanup */
    mg_mgr_free(&mgr);

    printf("Exiting on signal %d\n", s_sig_num);

    return 0;
}
