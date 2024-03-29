
#include <iostream>
#include "mongoose/mongoose.h"
#include "Networker/NetworkUtils.h"
#include "GenerateLink/FileEnumerator.h"
#include <filesystem>

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/hello")) {              // On /api/hello requests,
      mg_http_reply(c, 200, "", "{%m:%d}\n",
                    MG_ESC("status"), 1);                   // Send dynamic JSON response
    } else {                                                // For all other URIs,
      struct mg_http_serve_opts opts = {.root_dir = "../Resources"};   // Serve files
      mg_http_serve_dir(c, hm, &opts);                      // From root_dir
    }
  }
}

int main(int argc, const char * argv[]) {

    auto result = NetworkUtils::GetAndPrintLocalIpv4Addresses();

    FileEnumerator *enumerator = new FileEnumerator();
    enumerator->enumerate("../Resources");

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);                                      // Init manager
    mg_http_listen(&mgr, NetworkUtils::GetLocalHostUrlString().data(), fn, &mgr);  // Setup listener
    for (;;) mg_mgr_poll(&mgr, 1000);                       // Event loop
    mg_mgr_free(&mgr);                                      // Cleanup
    return 0;
}
