/*********************************************************************
 This is an example for our WICED Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This example uses the AdafruitHTTPServer class to create a simple 
 * webserver
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <adafruit_feather.h>
#include <adafruit_featherap.h>
#include <adafruit_http_server.h>
#include <Adafruit_NeoPixel.h>

#define WLAN_SSID            "jinxlights"
#define WLAN_PASS            "lightsgo"
#define WLAN_ENCRYPTION       ENC_TYPE_WPA2_AES
#define WLAN_CHANNEL          11

#define PORT                 80            // The TCP port to use
#define MAX_CLIENTS          10

IPAddress apIP     (192, 168, 2, 1);
IPAddress apGateway(192, 168, 2, 1);
IPAddress apNetmask(255, 255, 255, 0);

#define PIN                  PC7
#define NUMPIXELS            10

int ind1;
int ind2;
int ind3;
String Red   = "0";
String Green = "0";
String Blue  = "0";
int iRed     = 0;
int iGreen   = 0;
int iBlue    = 0;

Adafruit_SSD1306 oled = Adafruit_SSD1306();
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int visit_count = 0;

void about_generator (const char* url, const char* query, httppage_request_t* http_request);
void lights_page (const char* url, const char* query, httppage_request_t* http_request);
void file_not_found_generator (const char* url, const char* query, httppage_request_t* http_request);

const char hello_html[] = "<html><body> <h1>Goto /lights to change lights up.</h1> </body></html>";

HTTPPage pages[] = 
{
  HTTPPageRedirect("/", "/lights"), // redirect root to lights page
  HTTPPage("/hello.html", HTTP_MIME_TEXT_HTML, hello_html),
  HTTPPage("/about", HTTP_MIME_TEXT_HTML, about_generator),
  HTTPPage("/lights", HTTP_MIME_TEXT_HTML, lights_page),
  HTTPPage("/404.html" , HTTP_MIME_TEXT_HTML, file_not_found_generator),
};

uint8_t pagecount = sizeof(pages)/sizeof(HTTPPage);

// Declare HTTPServer with max number of pages
AdafruitHTTPServer httpserver(pagecount, WIFI_INTERFACE_SOFTAP);

/**************************************************************************/
/*!
 * @brief  Example of generating dynamic HTML content on demand
 * Link is separated to url and query
 *
 * @param url           url of this page
 * @param query         query string after '?' e.g "var=value"
 *
 * @param http_request  This request's information
*/
/**************************************************************************/
void about_generator (const char* url, const char* query, httppage_request_t* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  visit_count++;

  int heap_used_size = Feather.dbgHeapUsed();
  int heap_free_size = Feather.dbgHeapFree();

  // Note that all double quotes have been replaced with single quotes in
  // the HTML code below, taken from the default index.html template file.
  // Some double quotes and backslashes have also been escaped ('\' to '\\',
  // '"' to '\"') where the character needing an escape code couldn't be
  // avoided.

  httpserver.print("<!doctype html>\n"
                   "<html class='no-js' lang=''>\n"
                   "    <head>\n"
                   "        <meta charset='utf-8'>\n"
                   "        <meta http-equiv='x-ua-compatible' content='ie=edge'>\n"
                   "        <title></title>\n"
                   "        <meta name='description' content=''>\n"
                   "        <meta name='viewport' content='width=device-width, initial-scale=1'>\n"
                   "        <link rel='apple-touch-icon' href='apple-touch-icon.png'>\n"
                   "        <link rel='stylesheet' href='css/normalize.css'>\n"
                   "        <link rel='stylesheet' href='css/main.css'>\n"
                   "        <script src='js/vendor/modernizr-2.8.3.min.js'></script>\n"
                   "    </head>\n"
                   "    <body>\n"
                   "        <!--[if lt IE 8]>\n"
                   "            <p class='browserupgrade'>You are using an <strong>outdated</strong> browser. Please <a href='http://browsehappy.com/'>upgrade your browser</a> to improve your experience.</p>\n"
                   "        <![endif]-->\n");

  // Insert the dynamic body content here
  httpserver.print("        <h2>Module Details</h2>\n");
  httpserver.print("        <ul>\n");
  httpserver.print("            <li><b>Bootloader</b> : "); httpserver.print( Feather.bootloaderVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>WICED SDK</b> : "); httpserver.print( Feather.sdkVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>FeatherLib</b> : "); httpserver.print( Feather.firmwareVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>Arduino API</b> : "); httpserver.print( Feather.arduinoVersion() ); httpserver.print("</li>\n");
  httpserver.print("            <li><b>Heap Used</b> : "); httpserver.print( heap_used_size ); httpserver.print(" bytes</li>\n");
  httpserver.print("            <li><b>Heap Free</b> : "); httpserver.print( heap_free_size ); httpserver.print(" bytes</li>\n");
  httpserver.print("        </ul>\n");
  httpserver.print("        <h2>Page Details</h2>\n");
  httpserver.print("        <ul>\n");
  httpserver.print("            <li><b>Page Loads</b> : "); httpserver.print( visit_count ); httpserver.print("</li>\n");
  httpserver.print("        </ul>\n");
  httpserver.print("        <p>This page has been dynamically generated.</p>\n");

  // Take note of the escaped backslash and double-quotes in the text below
  httpserver.print("        <script src='https://code.jquery.com/jquery-1.12.0.min.js'></script>\n"
                   "        <script>window.jQuery || document.write('<script src=\"js/vendor/jquery-1.12.0.min.js\"><\\/script>')</script>\n"
                   "        <script src='js/plugins.js'></script>\n"
                   "        <script src='js/main.js'></script>\n"
                   "        <!-- Google Analytics: change UA-XXXXX-X to be your site's ID. -->\n"
                   "        <script>\n"
                   "            (function(b,o,i,l,e,r){b.GoogleAnalyticsObject=l;b[l]||(b[l]=\n"
                   "            function(){(b[l].q=b[l].q||[]).push(arguments)});b[l].l=+new Date;\n"
                   "            e=o.createElement(i);r=o.getElementsByTagName(i)[0];\n"
                   "            e.src='https://www.google-analytics.com/analytics.js';\n"
                   "            r.parentNode.insertBefore(e,r)}(window,document,'script','ga'));\n"
                   "            ga('create','UA-XXXXX-X','auto');ga('send','pageview');\n"
                   "        </script>\n"
                   "    </body>\n"
                   "</html>\n");
}

void lights_page (const char* url, const char* query, httppage_request_t* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;

  boolean hasQuery = false;
  String theQuery = String(query);
  
  if (theQuery.length()){
    hasQuery = true;
    ind1   = theQuery.indexOf("&");
    ind2   = theQuery.indexOf("&", ind1+1);
    ind3   = theQuery.indexOf("&", ind2+1);
    Red    = theQuery.substring(2,ind1);
    Green  = theQuery.substring(ind1+3, ind2);
    Blue   = theQuery.substring(ind2+3, ind3);
    iRed   = Red.toInt();
    iGreen = Green.toInt();
    iBlue  = Blue.toInt();
    colorWipe(strip.Color(iRed, iGreen, iBlue), 10);
  } else {
    colorWipe(strip.Color(iRed, iGreen, iBlue), 10);
  }
  updateOled();
  
  httpserver.print("<!DOCTYPE html>\n"
                   "  <html lang='en'>\n"
                   "    <head>\n"
                   "      <meta charset='utf-8'>\n"
                   "      <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>\n"
                   "      <title>Light Color Controller</title>\n"
                   "    </head>\n"
                   "    <body>\n"
                   "      <center>\n"
                   "        <p>");
  httpserver.print(Red + " - " + Green + " - " + Blue);                 
  httpserver.print("        </p>\n"
                   "        <label for='colorWell'>Color:</label>\n"
                   "        <input type='color' value='#ff0000' id='colorWell'>\n"
                   "      </center>\n"
                   "      <script type='application/javascript'>\n"
                   "        var colorWell;\n"
                   "        var defaultColor = '#ffffff';\n"
                   "        var rgbToHex = function (rgb) {\n"
                   "          var hex = Number(rgb).toString(16);\n"
                   "          if (hex.length < 2) {\n"
                   "               hex = '0' + hex;\n"
                   "          }\n"
                   "          return hex;\n"
                   "        };\n\n"
                   "        var fullColorHex = function(r,g,b) {\n"
                   "          var red = rgbToHex(r);\n"
                   "          var green = rgbToHex(g);\n"
                   "          var blue = rgbToHex(b);\n"
                   "          return '#' + red+green+blue;\n"
                   "        };\n\n"
                   "        function hex2rgb(hexStr){\n"
                   "          // note: hexStr should be #rrggbb\n"
                   "          var hex = parseInt(hexStr.substring(1), 16);\n"
                   "          var r = (hex & 0xff0000) >> 16;\n"
                   "          var g = (hex & 0x00ff00) >> 8;\n"
                   "          var b = hex & 0x0000ff;\n"
                   "          return [r, g, b];\n"
                   "        }\n\n"
                   "      window.addEventListener('load', startup, false);\n\n"
                   "      function startup() {\n"
                   "        colorWell = document.querySelector('#colorWell');\n"
                   "        theBody = document.body;\n"
                   "        var defaulter = fullColorHex("); httpserver.print(Red + "," + Green + "," + Blue); httpserver.print(");\n");
  httpserver.print("        colorWell.value = defaulter;\n"
                   "        colorWell.addEventListener('input', updateFirst, false);\n"
                   "        colorWell.addEventListener('change', updateAll, false);\n"
                   "        colorWell.select();\n"
                   "      }\n\n"
                   "      function updateFirst(event) {\n"
                   "        document.body.style.background = event.target.value;\n"
                   "      //var p = document.querySelector('p');\n"
                   "      }\n\n"
                   "      function updateAll(event) {\n"
                   "        //document.body.style.background = event.target.value;\n"
                   "        defaultColor = event.target.value;\n"
                   "        var rgb = hex2rgb(defaultColor);\n"
                   "        //console.log(rgb);\n"
                   "        window.location.href = '/lights' + '?r=' + rgb[0] + '&g=' + rgb[1] + '&b=' + rgb[2];\n"
                   "      }\n"
                   "    </script>\n"
                   "  </body>\n"
                   "</html>\n");
  
}

/**************************************************************************/
/*!
 * @brief  HTTP 404 generator. The HTTP Server will automatically redirect
 *         to "/404.html" when it can't find the requested url in the
 *         list of registered pages
 *
 * The url and query string are already separated when this function
 * is called.
 *
 * @param url           url of this page
 * @param query         query string after '?' e.g "var=value"
 * @param http_request  Details about this HTTP request
*/
/**************************************************************************/
void file_not_found_generator (const char* url, const char* query, httppage_request_t* http_request)
{
  (void) url;
  (void) query;
  (void) http_request;
 
  httpserver.print("<html><body>");
  httpserver.print("<h1>Error 404 File Not Found!</h1>");
  httpserver.print("<br>");

  httpserver.print(query);
  httpserver.print("<br>");
  
  httpserver.print("Available pages are:");
  httpserver.print("<br>");
  
  // Show a link list of all available pages:
  httpserver.print("<ul>");
  for(int i=0; i<pagecount; i++)
  {
    httpserver.print("<li>");
    httpserver.print(pages[i].url);
    httpserver.print("</li>");
  }
  httpserver.print("</ul>");
  
  httpserver.print("</body></html>");
}

/**************************************************************************/
/*!
    @brief  The setup function runs once when the board comes out of reset
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Setup the OLED display
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);

  Serial.println("SoftAP Simple HTTP Server Example\r\n");
  
  // Print all software versions
  Feather.printVersions();

  Serial.println("Configuring SoftAP\r\n");
  FeatherAP.err_actions(true, true);
  FeatherAP.setJoinCallback(softap_join_callback);
  FeatherAP.setLeaveCallback(softap_leave_callback);
  FeatherAP.begin(apIP, apGateway, apNetmask, WLAN_CHANNEL);

  Serial.println("Starting SoftAP\r\n");
  FeatherAP.start(WLAN_SSID, WLAN_PASS, WLAN_ENCRYPTION);
  FeatherAP.printNetwork();

  // Tell the HTTP client to auto print error codes and halt on errors
  httpserver.err_actions(true, true);

  // Configure HTTP Server Pages
  Serial.println("Adding Pages to HTTP Server");
  httpserver.addPages(pages, pagecount);
  httpserver.setConnectCallback(httpd_connect_callback);
  httpserver.setDisconnectCallback(httpd_disconnect_callback);

  Serial.print("Starting HTTP Server ... ");
  httpserver.begin(PORT, MAX_CLIENTS);
  Serial.println(" running");

  updateOled();
  colorWipe(strip.Color(iRed, iGreen, iBlue), 10);
  
}

void updateOled(){
  uint32_t ipAddress = apIP;
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("SSID  : ");
  oled.println(WLAN_SSID);
  oled.print("PASS  : ");
  oled.println(WLAN_PASS);
  oled.print("URL   : ");
  oled.print((ipAddress >> 24) & 0xFF, DEC);
  oled.print(".");
  oled.print((ipAddress >> 16) & 0xFF, DEC);
  oled.print(".");
  oled.print((ipAddress >> 8) & 0xFF, DEC);
  oled.print(".");
  oled.println(ipAddress & 0xFF, DEC);
  oled.print("COLOR : ");
  oled.print(iRed);
  oled.print(",");
  oled.print(iGreen);
  oled.print(",");
  oled.print(iBlue);
  oled.display();
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  togglePin(BOARD_LED_PIN);
  delay(1000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

/**************************************************************************/
/*!
    @brief  HTTP Server connect callback
*/
/**************************************************************************/
void httpd_connect_callback(uint32_t remote_ip, uint16_t remote_port)
{
  IPAddress ipaddr(remote_ip);
  Serial.print("HTTPD Connect   : client IP = ");
  Serial.print(ipaddr);
  Serial.print(" port = ");
  Serial.println(remote_port);
}

/**************************************************************************/
/*!
    @brief  HTTP Server disconnect callback
*/
/**************************************************************************/
void httpd_disconnect_callback(uint32_t remote_ip, uint16_t remote_port)
{
  IPAddress ipaddr(remote_ip);
  Serial.print("HTTPD Disconnect: client IP = ");
  Serial.print(ipaddr);
  Serial.print(" port = ");
  Serial.println(remote_port);
}

/**************************************************************************/
/*!
    @brief  SoftAP Join callback
*/
/**************************************************************************/
void softap_join_callback(const uint8_t mac[6], uint32_t ipv4)
{
  IPAddress ipaddr(ipv4);
  Serial.print("SoftAP joined: ");
  Serial.println(ipaddr);
}

/**************************************************************************/
/*!
    @brief  SoftAP Leave callback
*/
/**************************************************************************/
void softap_leave_callback(const uint8_t mac[6], uint32_t ipv4)
{
  IPAddress ipaddr(ipv4);
  Serial.print("SoftAP left  : ");
  Serial.println(ipaddr);
}



