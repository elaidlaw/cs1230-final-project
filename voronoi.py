import numpy as np
import cv2
from random import random
import math

tile_width = 100
tile_height = 100

# mapping from (vert 1 index, vert 2 index) to [vert3 index, vert 4 index]
edge_tris = {}

# list of (vert 1 index, vert 2 index, vert 3 index)
tris = []

# voronoi diagram sites
verts = [(0, 0), (0, tile_height), (tile_width, tile_height), (tile_width, 0)]

# vertices in the voronoi diagram graph
vverts = []

# edges in the voronoi diagram graph
# edges are of the form (vvert 1 index, vvert2 index)
vedges = []

# corresponds to edges, [vert 1 index, maybe vert 2 index]
# shows what cell this edge is part of
vedge_cells = []

# points to add to the diagram
points = []

# corresponds to verts, a list of the vedge indices that this cell includes
cells = [[], [], [], []]

buildings = []


for i in range(5):
    points.append((random() * tile_width, random() * tile_height))

def insert_tri(tri):
    tri = tuple(sorted(tri))
    tris.append(tri)
    for triple in [tri, (tri[0], tri[2], tri[1]), (tri[1], tri[2], tri[0])]:
        if triple[:2] not in edge_tris:
            edge_tris[triple[:2]] = []
        edge_tris[triple[:2]].append(triple[2])
    
def remove_tri(tri):
    tri = tuple(sorted(tri))
    tris.remove(tri)
    for triple in [tri, (tri[0], tri[2], tri[1]), (tri[1], tri[2], tri[0])]:
        if triple[:2] not in edge_tris:
            continue
        edge_tris[triple[:2]].remove(triple[2])
        if len(edge_tris[triple[:2]]) == 0:
            del edge_tris[triple[:2]]

def display(ms, show_delaunay=True, iterate_over_cells=False):
    image = np.zeros((1000, 1000, 3))

    bounds = (tile_width, tile_height)

    def transform_point(point):
        return (int(point[0] * 800 / bounds[0] + 100), int(point[1] * 800 / bounds[1] + 100))
    
    if show_delaunay:
        for edge in edge_tris:
            if len(edge_tris[edge]) == 0:
                continue
            image = cv2.line(image, transform_point(verts[edge[0]]), transform_point(verts[edge[1]]), (255, 0, 0), 1)
        for v in vverts:
            image = cv2.circle(image, transform_point(v), 2, (0, 255, 255), -1)
    else:
        for v in verts:
            image = cv2.circle(image, transform_point(v), 2, (255, 0, 0), -1)
            
    if iterate_over_cells:
        for cell in cells:
            color = (random(), random(), random())
            for edge in [vedges[i] for i in cell]:
                image = cv2.line(image, transform_point(vverts[edge[0]]), transform_point(vverts[edge[1]]), color, 1)
            cv2.imshow('image', image)
            cv2.waitKey(-1)
    else:
        for edge in vedges:
            if edge is None:
                continue

            image = cv2.line(image, transform_point(vverts[edge[0]]), transform_point(vverts[edge[1]]), (0, 255, 255), 1)
    
    for b in buildings:
        image = cv2.circle(image, transform_point(b[0]), 3, (255, 0, 255), -1)
    
    cv2.imshow("image", image) 

    cv2.waitKey(ms)

def triangle_contains(tri, p):
    p0, p1, p2 = [verts[i] for i in tri]
    A = 0.5 * (-p1[1] * p2[0] + p0[1] * (-p1[0] + p2[0]) + p0[0] * (p1[1] - p2[1]) + p1[0] * p2[1])
    sign = -1 if A < 0 else 1
    s = (p0[1] * p2[0] - p0[0] * p2[1] + (p2[1] - p0[1]) * p[0] + (p0[0] - p2[0]) * p[1]) * sign
    t = (p0[0] * p1[1] - p0[1] * p1[0] + (p0[1] - p1[1]) * p[0] + (p1[0] - p0[0]) * p[1]) * sign
    
    return s > 0 and t > 0 and (s + t) < 2 * A * sign

def insert_vert(p):
    verts.append(p)
    cells.append([])
    ind = len(verts) - 1
    for tri in tris:
        if triangle_contains(tri, p):
            remove_tri(tri)
            insert_tri((tri[0], tri[1], ind))
            insert_tri((tri[0], tri[2], ind))
            insert_tri((tri[1], tri[2], ind))
            break

def insert_vedge(vedge, edge_cells):
    vedges.append(vedge)
    vedge_cells.append(edge_cells)
    for cell in edge_cells:
        cells[cell].append(len(vedges) - 1)

def remove_vedge(ind):
    edge_cells = vedge_cells[ind]
    for cell in edge_cells:
        cells[cell].remove(ind)
    vedges[ind] = None
    vedge_cells[ind] = None

display(-1)

insert_tri((0, 1, 2))
insert_tri((0, 3, 2))

for p in points:
    insert_vert(p)
    display(100)

stack = [e for e in edge_tris]
marked = set(stack)

def get_angle(angle):
    a = [verts[i] for i in angle]
    def d(p1, p2):
        return np.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)

    return np.arccos((d(a[1], a[0]) ** 2 + d(a[1], a[2]) ** 2 - d(a[0], a[2]) ** 2) / (2 * d(a[1], a[0]) * d(a[1], a[2])))

while len(stack) > 0:
    e = stack.pop(-1)
    marked.remove(e)
    if e not in edge_tris or len(edge_tris[e]) < 2:
        continue
    p1 = edge_tris[e][0]
    p2 = edge_tris[e][1]
    if get_angle((e[0], p1, e[1])) + get_angle((e[0], p2, e[1])) > 3.141592653:
        remove_tri((e[0], p1, e[1]))
        remove_tri((e[0], p2, e[1]))
        insert_tri((p1, e[0], p2))
        insert_tri((p1, e[1], p2))
        outer_edges = [(p1, e[0]), (p2, e[0]), (p1, e[1]), (p2, e[1])]
        outer_edges = [tuple(sorted(outer_edge)) for outer_edge in outer_edges]
        for outer_edge in outer_edges:
            if outer_edge in marked:
                continue
            marked.add(outer_edge)
            stack.append(outer_edge)
        display(100)

for tri in tris:
    p0, p1, p2 = [verts[i] for i in tri]

    a, b = ((p0[0] + p1[0]) / 2, (p0[1] + p1[1]) / 2)
    c, d = ((p1[0] + p2[0]) / 2, (p1[1] + p2[1]) / 2)
    ad, bd = (p1[1] - p0[1], p0[0] - p1[0])
    cd, dd = (p2[1] - p1[1], p1[0] - p2[0])

    s = (a * bd - c * bd + d * ad - b * ad) / (cd * bd - ad * dd)

    vverts.append((c + cd * s, d + dd * s))

    display(100)

tri_lookup = { tris[k]: k for k in range(len(tris))}

for e in edge_tris:
    if len(edge_tris[e]) < 2:
        continue
    v1 = tri_lookup[tuple(sorted((e[0], e[1], edge_tris[e][0])))]
    v2 = tri_lookup[tuple(sorted((e[0], e[1], edge_tris[e][1])))]
    insert_vedge((v1, v2), [e[0], e[1]])
    display(100)

def remove_out_of_bounds(lower_vertex, upper_vertex, lower, index, lvi, uvi):
    new_v = []
    new_v_cells = []
    for i, v in enumerate(vverts):
        if (v[index] < vverts[lower_vertex][index] and lower) or (v[index] > vverts[lower_vertex][index] and not lower):
            for j, e in enumerate(vedges):
                if e is None or i not in e:
                    continue
                edge_cells = vedge_cells[j]
                remove_vedge(j)
                p1 = v
                p2i = e[0] if e[0] != i else e[1]
                p2 = vverts[p2i]
                if (p2[index] < vverts[lower_vertex][index] and lower) or (p2[index] > vverts[lower_vertex][index] and not lower):
                    continue
                t = (vverts[lower_vertex][index] - p1[index]) / (p2[index] - p1[index])
                p3 = (p1[0] + (p2[0] - p1[0]) * t, p1[1] + (p2[1] - p1[1]) * t)
                vverts.append(p3)
                insert_vedge((p2i, len(vverts) - 1), edge_cells)
                if vverts[-1][(index + 1) % 2] > vverts[lower_vertex][(index + 1) % 2] and vverts[-1][(index + 1) % 2] < vverts[upper_vertex][(index + 1) % 2]:
                    new_v.append(len(vverts) - 1)
                    new_v_cells.append(edge_cells)
                display(100, False)
    sorted_new_v = [(lower_vertex, [lvi])] + sorted(zip(new_v, new_v_cells), key=lambda x: vverts[x[0]][(index + 1) % 2]) + [(upper_vertex, [uvi])]
    for i in range(len(sorted_new_v) - 1):
        insert_vedge((sorted_new_v[i][0], sorted_new_v[i + 1][0]), [x for x in sorted_new_v[i][1] if x in sorted_new_v[i + 1][1]])
        display(100, False)
    

vverts.append((0, 0))
vverts.append((tile_width, 0))
vverts.append((0, tile_height))
vverts.append((tile_width, tile_height))
top_left = len(vverts) - 4
top_right = len(vverts) - 3
bottom_left = len(vverts) - 2
bottom_right = len(vverts) - 1

remove_out_of_bounds(top_left, bottom_left, True, 0, 0, 1)
remove_out_of_bounds(top_left, top_right, True, 1, 0, 3)
remove_out_of_bounds(top_right, bottom_right, False, 0, 3, 2)
remove_out_of_bounds(bottom_left, bottom_right, False, 1, 1, 2)

def split_edge(edge_index, fraction):
    v1i = vedges[edge_index][0]
    v2i = vedges[edge_index][1]
    v1 = vverts[vedges[edge_index][0]]
    v2 = vverts[vedges[edge_index][1]]
    v3 = (v1[0] + fraction * (v2[0] - v1[0]), v1[1] + fraction * (v2[1] - v1[1]))
    vverts.append(v3)
    edge_cells = vedge_cells[edge_index]
    remove_vedge(edge_index)
    insert_vedge((v1i, len(vverts) - 1), edge_cells)
    insert_vedge((v2i, len(vverts) - 1), edge_cells)

def get_edge_limits(edge, slope):
    v1 = vverts[edge[0]]
    v2 = vverts[edge[1]]
    
    limit_v1 = -slope * v1[0] + v1[1]
    limit_v2 = -slope * v2[0] + v2[1]

    return min(limit_v1, limit_v2), max(limit_v1, limit_v2)


def insert_grid_edges(slope, spacing, cell_index):
    out_edges = []
    edge_limits = {edge: get_edge_limits(vedges[edge], slope) for edge in cells[cell_index]}
    limits = min([edge_limits[e][0] for e in edge_limits]), max([edge_limits[e][1] for e in edge_limits])
    for i in range(math.ceil(limits[0] / spacing), math.floor(limits[1] / spacing) + 1):
        edges = [edge for edge in cells[cell_index] if i * spacing > edge_limits[edge][0] and i * spacing <= edge_limits[edge][1]]
        for edge in edges:
            v1 = vverts[vedges[edge][0]]
            v2 = vverts[vedges[edge][1]]
            
            limit_v1 = -slope * v1[0] + v1[1]
            limit_v2 = -slope * v2[0] + v2[1]

            split_edge(edge, (i * spacing - limit_v1) / (limit_v2 - limit_v1))
            del edge_limits[edge]
            edge_limits[len(vedges) - 1] = get_edge_limits(vedges[-1], slope)
            edge_limits[len(vedges) - 2] = get_edge_limits(vedges[-2], slope)
        insert_vedge((len(vverts) - 1, len(vverts) - 2), [])
        out_edges.append((i, len(vedges) - 1))
        display(100, False)
    return out_edges

def compute_spacing(m, x):
    m = abs(m)
    return m * math.sqrt((x / m) ** 2 + x ** 2)

def get_grid_intersection(i, j, slope1, slope2, spacing1, spacing2):
    x = (j * spacing2 - i * spacing1) / (slope1 - slope2)
    y = slope1 * x + i * spacing1

    return x, y

for i in range(len(cells)):
    slope1 = random() * 0.7 - 0.35
    slope2 = -1 / (slope1)

    building_angle = math.atan(slope1)
    
    width1 = random() * 8 + 4
    width2 = random() * 8 + 4
    spacing1 = compute_spacing(slope1, width1)
    spacing2 = compute_spacing(slope2, width2)
    edges1 = insert_grid_edges(slope1, spacing1, i)
    edges2 = insert_grid_edges(slope2, spacing2, i)
    corner_points = []
    for gedge in edges1:
        limits = get_edge_limits(vedges[gedge[1]], slope2)
        corner_points += [(gedge[0], j) for j in range(math.ceil(limits[0] / spacing2), math.floor(limits[1] / spacing2) + 1)]
    # for cp in corner_points:
    #     buildings.append(get_grid_intersection(cp[0], cp[1], slope1, slope2, spacing1, spacing2))
    #     print(buildings[-1])
    #     display(10, False)
    full_squares = []
    for j, k in corner_points:
        if (j + 1, k) in corner_points and (j, k + 1) in corner_points and (j + 1, k + 1) in corner_points:
            full_squares.append((j, k))
            new_buildings = [(j + 0.5, k + 0.5, (width1 - 2.2) / width1, (width2 - 2.2) / width2)]
            for l in range(int(random() * 5)):
                ind = int(random() * len(new_buildings))
                pi, pj, w, h = new_buildings[ind]
                p = (pi, pj)
                size = (w, h)
                w_to_use = width1
                flipped = False
                if random() < 0.5:
                    flipped = True
                    w_to_use = width2
                    p = (p[1], p[0])
                    size = (size[1], size[0])
                frac = random() * 0.5 + 0.25
                new_building1 = [0, p[1], 0, size[1]]
                new_building2 = [0, p[1], 0, size[1]]
                new_building1[0] = p[0] - size[0] / 2 + size[0] * frac / 2 - 0.05 / w_to_use
                new_building1[2] = size[0] * frac - 0.1 / w_to_use
                new_building2[0] = p[0] + size[0] / 2 - size[0] * (1 - frac) / 2 + 0.05 / w_to_use
                new_building2[2] = size[0] * (1 - frac) - 0.1 / w_to_use
                if new_building1[3] / new_building1[2] > 3 or new_building1[2] / new_building1[3] > 3 or new_building2[3] / new_building2[2] > 3 or new_building2[2] / new_building2[3] > 3:
                    continue
                if flipped:
                    new_building1 = (new_building1[1], new_building1[0], new_building1[3], new_building1[2])
                    new_building2 = (new_building2[1], new_building2[0], new_building2[3], new_building2[2])
                new_buildings[ind] = tuple(new_building1)
                new_buildings.append(tuple(new_building2))

            for b in new_buildings:
                center = get_grid_intersection(b[0], b[1], slope1, slope2, spacing1, spacing2)
                buildings.append((center, building_angle, (b[2] * width1, b[3] * width2)))

            # first_corner = get_grid_intersection(j, k, slope1, slope2, spacing1, spacing2)
            # second_corner = get_grid_intersection(j + 1, k + 1, slope1, slope2, spacing1, spacing2)
            # buildings.append((((first_corner[0] + second_corner[0]) / 2, (first_corner[1] + second_corner[1]) / 2), building_angle, ((width1 - 2.2) / width1, (width2 - 2.2) / width2)))
            display(100, False)

display(-1, False)

x = input()
if x != '':
    with open(x, 'w+') as f:
        for vvert in vverts:
            f.write(f'v {vvert[0]} {vvert[1]}\n')
        for vedge in vedges:
            if vedge is None:
                continue
            f.write(f'e {vedge[0]} {vedge[1]}\n')
        for building in buildings:
            f.write(f'b {building[0][0]} {building[0][1]} {building[1]} {building[2][0]} {building[2][1]}\n')