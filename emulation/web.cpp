#include <cstdlib>
#include <ctime>
#include "mongoose.h"

static const char *s_http_port = "http://0.0.0.0:8000/";

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data) {
    struct mg_http_serve_opts opts = { .root_dir = "." };  // Serve files from the upper directory

    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_http_match_uri(hm, "/temp/1")) {
            // Generate a random temperature between 175 and 260
            float randomTemp = 175 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(260-175)));
            char tempStr[10];
            int length = snprintf(tempStr, sizeof(tempStr), "%.2f", randomTemp);
            tempStr[length] = 0;
            char headers[50];
            snprintf(headers, sizeof(headers), "Content-Type: text/plain\r\nContent-Length: %d\r\n", length);
            mg_http_reply(nc, 200, headers, "%s", tempStr);
        } else if (mg_http_match_uri(hm, "/")) {
            // Serve index.html
            mg_http_serve_file(nc, hm, "index.html", &opts);
        }
    }
}

int main(void) {
    // Seed the random number generator
    srand(time(NULL));
    struct mg_mgr mgr;
    struct mg_connection *nc;

    mg_mgr_init(&mgr);
    nc = mg_http_listen(&mgr, s_http_port, ev_handler, NULL);

    if (nc == NULL) {
        printf("Failed to create listener\n");
        return 1;
    }

    printf("Starting web server on port %s\n", s_http_port);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);

    return 0;
}
