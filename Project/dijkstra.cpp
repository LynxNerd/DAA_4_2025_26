#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int INFINITY_VAL = numeric_limits<int>::max();

struct Edge {
    string destination;
    int distance;
};

unordered_map<string, vector<Edge>> graph;
unordered_map<string, pair<int, string>> distances; // distance, previous city

bool readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return false;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        stringstream ss(line);
        string from, to;
        int distance;
        
        if (ss >> from >> to >> distance) {
            graph[from].push_back({to, distance});
            graph[to].push_back({from, distance});
        }
    }
    
    file.close();
    return true;
}

vector<string> dijkstra(const string& source, const string& destination) {
    distances.clear();
    
    // Initialize distances
    for (auto& node : graph) {
        distances[node.first] = {INFINITY_VAL, ""};
    }
    
    distances[source] = {0, ""};
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    pq.push({0, source});
    
    while (!pq.empty()) {
        auto [dist, city] = pq.top();
        pq.pop();
        
        if (dist > distances[city].first) continue;
        
        for (auto& edge : graph[city]) {
            int newDist = distances[city].first + edge.distance;
            if (newDist < distances[edge.destination].first) {
                distances[edge.destination] = {newDist, city};
                pq.push({newDist, edge.destination});
            }
        }
    }
    
    // Reconstruct path
    vector<string> path;
    string current = destination;
    
    while (!current.empty()) {
        path.push_back(current);
        current = distances[current].second;
    }
    
    reverse(path.begin(), path.end());
    return path;
}

string generateJsonResponse(const vector<string>& path, int totalDistance) {
    string json = "{\"success\": true, \"path\": [";
    for (int i = 0; i < path.size(); i++) {
        json += "\"" + path[i] + "\"";
        if (i < path.size() - 1) json += ", ";
    }
    json += "], \"distance\": " + to_string(totalDistance) + ", \"cities\": " + to_string(path.size()) + "}";
    return json;
}

string generateErrorResponse(const string& error) {
    return "{\"success\": false, \"error\": \"" + error + "\"}";
}

void handleRequest(int clientSocket) {
    char buffer[4096] = {0};
    read(clientSocket, buffer, sizeof(buffer));
    
    string request(buffer);
    
    // Parse GET request
    size_t qpos = request.find('?');
    if (qpos == string::npos) {
        string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n\r\n";
        response += generateErrorResponse("Invalid request");
        write(clientSocket, response.c_str(), response.length());
        close(clientSocket);
        return;
    }
    
    string query = request.substr(qpos + 1);
    query = query.substr(0, query.find(' '));
    
    // Parse source and destination
    string source, destination;
    size_t srcPos = query.find("from=");
    size_t dstPos = query.find("to=");
    
    if (srcPos != string::npos) {
        size_t endPos = query.find("&", srcPos);
        if (endPos == string::npos) endPos = query.length();
        source = query.substr(srcPos + 5, endPos - srcPos - 5);
    }
    
    if (dstPos != string::npos) {
        size_t endPos = query.find("&", dstPos);
        if (endPos == string::npos) endPos = query.length();
        destination = query.substr(dstPos + 3, endPos - dstPos - 3);
    }
    
    // URL decode
    auto urlDecode = [](string& str) {
        string decoded;
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] == '+') decoded += ' ';
            else if (str[i] == '%' && i + 2 < str.length()) {
                string hex = str.substr(i + 1, 2);
                decoded += (char)stoi(hex, nullptr, 16);
                i += 2;
            } else {
                decoded += str[i];
            }
        }
        str = decoded;
    };
    
    urlDecode(source);
    urlDecode(destination);
    
    // Validate input
    if (source.empty() || destination.empty()) {
        string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
        response += generateErrorResponse("Missing source or destination");
        write(clientSocket, response.c_str(), response.length());
        close(clientSocket);
        return;
    }
    
    // Check if cities exist in graph
    if (graph.find(source) == graph.end() || graph.find(destination) == graph.end()) {
        string response = "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
        response += generateErrorResponse("City not found in graph");
        write(clientSocket, response.c_str(), response.length());
        close(clientSocket);
        return;
    }
    
    // Run Dijkstra's algorithm
    vector<string> path = dijkstra(source, destination);
    
    if (path.empty() || path[0] != source) {
        string response = "HTTP/1.1 404 Not Found\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
        response += generateErrorResponse("No path found");
        write(clientSocket, response.c_str(), response.length());
        close(clientSocket);
        return;
    }
    
    int totalDistance = distances[destination].first;
    string jsonResponse = generateJsonResponse(path, totalDistance);
    
    string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: " + to_string(jsonResponse.length()) + "\r\n\r\n";
    response += jsonResponse;
    
    write(clientSocket, response.c_str(), response.length());
    close(clientSocket);
}

int main() {
    if (!readGraphFromFile("graph.txt")) {
        cerr << "Failed to read graph data" << endl;
        return 1;
    }
    
    cout << "Graph loaded with " << graph.size() << " cities" << endl;
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error creating socket" << endl;
        return 1;
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error binding socket" << endl;
        return 1;
    }
    
    listen(serverSocket, 5);
    cout << "Server listening on http://localhost:8080" << endl;
    
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (clientSocket >= 0) {
            handleRequest(clientSocket);
        }
    }
    
    close(serverSocket);
    return 0;
}
