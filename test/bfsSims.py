import numpy as np
import random
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import numpy as np
import random

# Define possible directions: Right, Down, Left, Up
DIRECTIONS = [(0, 1), (1, 0), (0, -1), (-1, 0)]
size = 8

def generate_start(size):
    """
    Generate a random start position on the outer wall such that:
    - It is either on row 0, row (size-1), column 0, or column (size-1).
    - The adjacent position inside the maze must be in an odd row/column.
    """
    is_row = random.choice([True, False])  # Randomly choose row or column placement
    if is_row:
        row = random.choice([0, size - 1])  # Choose top or bottom row
        col = random.randrange(1, size - 1, 2)  # Ensure adjacent cell is odd
    else:
        col = random.choice([0, size - 1])  # Choose left or right column
        row = random.randrange(1, size - 1, 2)  # Ensure adjacent cell is odd

    return (row, col)

def generate_maze(n, startR, startC):
    """
    Generate a randomized (2n+1)x(2n+1) maze using depth-first search (DFS) where:
    - Every wall remains connected to the outer border.
    - The maze has one connected path without isolated walls.
    - A valid exit is placed opposite the start.
    """
    size = 2 * n + 1
    maze = np.ones((size, size), dtype=int)

    # Define movement directions (row, col)
    directions = [(0, 2), (2, 0), (0, -2), (-2, 0)]  # Right, Down, Left, Up

    # Recursive backtracking function to generate a maze
    def carve_path(r, c):
        maze[r, c] = 0  # Open the cell
        random.shuffle(directions)  # Shuffle directions for randomness
        for dr, dc in directions:
            nr, nc = r + dr, c + dc  # New row, new column
            wr, wc = r + dr // 2, c + dc // 2  # Wall row, wall column
            if 1 <= nr < size - 1 and 1 <= nc < size - 1 and maze[nr, nc] == 1:
                maze[wr, wc] = 0  # Open the wall
                carve_path(nr, nc)  # Recursive call

    # Start the maze generation from the top-left open cell
    carve_path(1, 1)

    # Ensure start position is open
    maze[startR, startC] = 0

    # Determine the exit row and column on the opposite side
    exitR, exitC = None, None
    candidates = []

    if startR == 0:  # Top border
        exitR = size - 1
        candidates = [j for j in range(1, size - 1, 2) if maze[exitR - 1, j] == 0]
    elif startR == size - 1:  # Bottom border
        exitR = 0
        candidates = [j for j in range(1, size - 1, 2) if maze[exitR + 1, j] == 0]
    elif startC == 0:  # Left border
        exitC = size - 1
        candidates = [i for i in range(1, size - 1, 2) if maze[i, exitC - 1] == 0]
    elif startC == size - 1:  # Right border
        exitC = 0
        candidates = [i for i in range(1, size - 1, 2) if maze[i, exitC + 1] == 0]

    # Set the exit position
    if candidates:
        exit_pos = random.choice(candidates)
        if exitR is not None:  # Horizontal exit
            maze[exitR, exit_pos] = 0
        else:  # Vertical exit
            maze[exit_pos, exitC] = 0

    return maze

# BFS function to explore the maze and find the exit
def bfs_explore(start, maze):
    rows, cols = maze.shape
    queue = deque([(start, [start])])  # Queue stores (current_position, path_to_current_position)
    visited = set()  # Track visited positions
    visited.add(start)
    explored = []  # Track all explored positions for visualization

    while queue:
        (r, c), path = queue.popleft()
        explored.append((r, c))  # Add current position to explored list

        # Check if current position is an exit
        if (r == 0 or r == rows - 1 or c == 0 or c == cols - 1) and maze[r][c] == 0 and (r, c) != start:
            return explored, path  # Return explored cells and the shortest path

        # Explore neighbors
        for dr, dc in DIRECTIONS:
            nr, nc = r + dr, c + dc
            if 0 <= nr < rows and 0 <= nc < cols and maze[nr][nc] == 0 and (nr, nc) not in visited:
                queue.append(((nr, nc), path + [(nr, nc)]))
                visited.add((nr, nc))

    return explored, []  # Return explored cells and an empty path if no exit is found

# Function to animate the exploration and solution path
def animate_solution(maze, explored, path):
    fig, ax = plt.subplots()
    ax.set_xticks([])
    ax.set_yticks([])
    ax.imshow((1 - maze), cmap='gray')  # Display the inverted maze

    # Initialize plots for exploration and path
    explored_x, explored_y = [], []
    path_x, path_y = [], []
    explored_plot, = ax.plot([], [], 'bo', markersize=5, alpha=0.5)  # Blue for explored cells
    path_plot, = ax.plot([], [], 'ro', markersize=5)  # Red for the shortest path

    # Function to update the animation frame
    def update(frame):
        if frame < len(explored):
            r, c = explored[frame]
            explored_x.append(c)
            explored_y.append(r)
            explored_plot.set_data(explored_x, explored_y)

        if frame >= len(explored) and frame - len(explored) < len(path):
            r, c = path[frame - len(explored)]
            path_x.append(c)
            path_y.append(r)
            path_plot.set_data(path_x, path_y)

        return explored_plot, path_plot

    # Create the animation
    total_frames = len(explored) + len(path)
    ani = animation.FuncAnimation(fig, update, frames=total_frames, interval=100, blit=True)

    # Save the animation
    writer = animation.FFMpegWriter(fps=10)
    ani.save("maze_exploration.mp4", writer=writer)

    plt.show()

# start = generate_start(size)
# real_maze = generate_maze(size,start[0],start[1])
start = (8,1)
real_maze = np.array([[1,1,1,1,1,1,1,0,1],
                     [1,0,1,0,0,0,0,0,1],
                     [1,0,1,0,1,0,1,0,1],
                     [1,0,1,0,1,0,1,0,1],
                     [1,0,1,1,1,1,1,0,1],
                     [1,0,0,0,0,0,1,0,1],
                     [1,0,1,1,1,0,1,0,1],
                     [1,0,1,0,0,0,0,0,1],
                     [1,0,1,1,1,1,1,1,1]])


# Run BFS and animate the exploration and solution
explored, path = bfs_explore(start, real_maze)
if path:
    print("Path to exit:", path)
    animate_solution(real_maze, explored, path)
else:
    print("No exit found.")