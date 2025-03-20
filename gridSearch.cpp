#include <iostream>
#include <vector>
#include <utility> 
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_set>

using namespace std;

enum Direction { UP, RIGHT, LEFT, DOWN };

const vector<pair<int, int>> DIRECTIONS = {
    {-1, 0}, // up
    {0, 1},  // right
    {0, -1}, // left
    {1, 0}   // down
};

//check to make sure that the next square is valid, not obstructed or out of bounds
bool isValidCell(int nextRow, int nextCol, int ROWS, int COLS, const vector<vector<int>> &grid) {
    return !(nextRow < 0 || nextRow >= ROWS || nextCol < 0 || nextCol >= COLS || grid[nextRow][nextCol] == 1);
}

vector<pair<int, int>> DFS(int sRow, int sCol, int gRow, int gCol, vector<vector<int>> &grid) {
    int ROWS = grid.size(), COLS = grid[0].size();
    stack<pair<int, int>> stack;
    vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    stack.push({sRow, sCol});
    visited[sRow][sCol] = true;

    while (!stack.empty()) {
        auto [currRow, currCol] = stack.top();
        stack.pop();

        if (currRow == gRow && currCol == gCol) {
            vector<pair<int, int>> path;
            while (currRow != -1 && currCol != -1) {
                path.push_back({currRow, currCol});
                tie(currRow, currCol) = parent[currRow][currCol];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 3; i >= 0; --i) { // Ensure correct order (Up → Right → Left → Down)
            int nextRow = currRow + DIRECTIONS[i].first;
            int nextCol = currCol + DIRECTIONS[i].second;

            if (isValidCell(nextRow, nextCol, ROWS, COLS, grid) && !visited[nextRow][nextCol]) {
                stack.push({nextRow, nextCol});
                visited[nextRow][nextCol] = true;
                parent[nextRow][nextCol] = {currRow, currCol};
            }
        }
    }
    return {};
}

vector<pair<int, int>> BFS(int sRow, int sCol, int gRow, int gCol, vector<vector<int>> &grid) {
    int ROWS = grid.size(), COLS = grid[0].size();
    queue<pair<int, int>> queue;
    vector<vector<pair<int, int>>> parent(ROWS, vector<pair<int, int>>(COLS, {-1, -1}));
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    queue.push({sRow, sCol});
    visited[sRow][sCol] = true;

    while (!queue.empty()) {
        auto [currRow, currCol] = queue.front();
        queue.pop();

        if (currRow == gRow && currCol == gCol) {
            vector<pair<int, int>> path;
            while (currRow != -1 && currCol != -1) {
                path.push_back({currRow, currCol});
                tie(currRow, currCol) = parent[currRow][currCol];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (const auto &dir : DIRECTIONS) {
            int nextRow = currRow + dir.first;
            int nextCol = currCol + dir.second;

            if (isValidCell(nextRow, nextCol, ROWS, COLS, grid) && !visited[nextRow][nextCol]) {
                queue.push({nextRow, nextCol});
                visited[nextRow][nextCol] = true;
                parent[nextRow][nextCol] = {currRow, currCol};
            }
        }
    }
    return {};
}

bool depthLimitedDFS(int sRow, int sCol, int gRow, int gCol, const vector<vector<int>> &grid, int limit, vector<pair<int, int>> &path) {

    int ROWS = grid.size(), COLS = grid[0].size();
    stack<tuple<int, int, int>> stack;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    stack.push({sRow, sCol, 0});
    visited[sRow][sCol] = true;

    while (!stack.empty()) {
        auto [currRow, currCol, depth] = stack.top();
        stack.pop();

        path.push_back({currRow, currCol});
        if (currRow == gRow && currCol == gCol) return true;

        if (depth < limit) {
            for (int i = 3; i >= 0; --i) {
                int nextRow = currRow + DIRECTIONS[i].first;
                int nextCol = currCol + DIRECTIONS[i].second;

                if (isValidCell(nextRow, nextCol, ROWS, COLS, grid) && !visited[nextRow][nextCol]) {
                    stack.push({nextRow, nextCol, depth + 1});
                    visited[nextRow][nextCol] = true;
                }
            }
        }
        path.pop_back();
    }
    return false;
}

vector<pair<int, int>> IDDFS(int sRow, int sCol, int gRow, int gCol, const vector<vector<int>> &grid, int &depthReached) {
    for (int limit = 0; limit < grid.size() * grid[0].size(); limit++) {
        vector<pair<int, int>> path;
        if (depthLimitedDFS(sRow, sCol, gRow, gCol, grid, limit, path)) {
            depthReached = limit;
            return path;
        }
    }
    return {};
}

int main() {
    int x, y;
    cout << "Enter grid dimensions (height width): ";
    cin >> y >> x;

    vector<vector<int>> grid(x, vector<int>(y, 0));
    cout << "How many squares are forbidden? ";
    int numObstacles;
    cin >> numObstacles;

    cout << "Enter forbidden coordinates (x y): ";
    for (int i = 0; i < numObstacles; i++) {
        int row, col;
        cin >> row >> col;
        grid[row][col] = 1;
    }

    cout << "Enter start and goal positions (start_x start_y goal_x goal_y): ";
    int sRow, sCol, gRow, gCol;
    cin >> sRow >> sCol >> gRow >> gCol;

    vector<pair<int, int>> dfsPath = DFS(sRow, sCol, gRow, gCol, grid);
    vector<pair<int, int>> bfsPath = BFS(sRow, sCol, gRow, gCol, grid);
    
    int depthReached = 0;
    vector<pair<int, int>> iddfsPath = IDDFS(sRow, sCol, gRow, gCol, grid, depthReached);

    cout << "DFS Path: "; for (auto p : dfsPath) cout << "(" << p.first << "," << p.second << ") " << '\n';
    cout << "BFS Path: "; for (auto p : bfsPath) cout << "(" << p.first << "," << p.second << ") " << '\n';
    cout << "IDDFS Path: "; for (auto p : iddfsPath) cout << "(" << p.first << "," << p.second << ") " << '\n';
    cout << "Solution reached at depth: " << depthReached << endl;

    return 0;
}