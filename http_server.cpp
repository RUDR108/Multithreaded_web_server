#include "http_server.hh"
#include<bits/stdc++.h>
#include <vector>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include<string>


vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;
  //cout<<s;
  while (getline(ss, item, ':')) {
    
    if (!item.empty())
      {
        //cout<<item;
      elems.push_back(item);
      }
    
  }
  
  return elems;
}
vector<string> split1(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;
  //cout<<s;
  while (getline(ss, item, ' ')) {
    
    if (!item.empty())
      {
        //cout<<item;
      elems.push_back(item);
      }
    
  }
  
  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  //cout<<request;
vector<string> lines = split(request, '\n');
vector<string> first_line = split1(lines[0], ' ');

  //cout<<request;
  //cout<<lines[0]<<endl<<"Abbay";
  //cout<<lines[1];

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request
  this->method=first_line[0];
  //cout<<first_line[0]<<"  ";
  this->url=first_line[1];

  //cout<<first_line[1]<<"cdcdc"; 
 /*
   TODO : extract the request method and URL from first_line here
  */
  

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    exit(1);
  }
}

HTTP_Response *handle_request(string req) {
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

 string url = string("html_files") + request->url;
 
 string url2;
  response->HTTP_version = "1.0";
  
//cout<<"Working Here";
  struct stat sb;
   if (stat(url.c_str(), &sb) == 0) // requested path exists
   {
    //cout<<"cwdcdvcd";
     response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";

     string body;

    if (S_ISDIR(sb.st_mode)) {
    
  
 
    url= url+"index.html"; 
    
    }
   
    //cout<<url;
    string row;
    string data;  
    ifstream MyReadFile(url);
    while (getline (MyReadFile, row)) {
      data+=row;
      data+="\n";
    }
        response->content_length=to_string(data.length());
    string str=response->get_string();
    
    response->body+=str;
    data+="\n\n";
    response->body+=data;
  
  
  }

  else {
    
    response->status_code = "404";
    response->status_text = "Not Found";
    response->content_type="text/html";
    string row;
    string data;
    fstream my_file;
	my_file.open("errorFile.html");
  while (getline (my_file, row))  {
      data+=row;
      data+="\n";
    }
  response->content_length=to_string(data.length());
  string str=response->get_string();
  response->body+=str;
   response->body+="\n\n";
  response->body+=data;
  
    /*
    TODO : set the remaining fields of response appropriately
    */
  }
  delete request;

  return response;
  
  
}

string HTTP_Response::get_string() {
 string str="HTTP/";
 str+=HTTP_version;
 str+=" ";
 str+=status_code;
 str+=" ";
 str+=status_text;
 str+="\n";
 str+="Content-type:";
 str+=content_type;
 str+="\n";
 str+="Content-length:";
 str+=content_length;
 str+="\n";
  /*
  TODO : implement this function
  */

 return str;
}