import math
import taichi as ti
ti.init(arch = ti.gpu)

res = (1600, 900)
pixels = ti.Vector.field(n = 3, dtype = float, shape = res)

gui = ti.GUI("test", res, fast_gui = True)

FPS = 60.0
dt = 1 / FPS
#camera params
FOV = 120
FOV_tan = math.tan(FOV / 360 * math.pi)

z_near, z_far = 100, 3200 

grid_length = 100
N = (res[0] // grid_length, res[1] // grid_length, (z_far-z_near) // grid_length)
colors = ti.Vector.field(n = 3, dtype = float, shape = N)
pos = ti.Vector.field(n = 3, dtype = float, shape = N)
velocity = ti.Vector.field(n = 3, dtype = float, shape = ())

# random color 
@ti.func 
def rand3():
    return ti.Vector([ti.random(), ti.random(), ti.random()]) # + ti.Vector([1., 2., 3.])

# 3d to 2d
@ti.func
def project(pos_3d):
    center = ti.Vector(res) / 2
    w = FOV_tan * pos_3d.z
    pos_2d = (ti.Vector([pos_3d.x, pos_3d.y]) - center) / w * res[1] + center
    return pos_2d
    
@ti.func
def draw_circle(point, rad, color):
    x_range = (ti.max(0, int(point.x - rad - 1)), ti.min(int(point.x + rad + 1), res[0]))
    y_range = (ti.max(0, int(point.y - rad - 1)), ti.min(int(point.y + rad + 1), res[1]))
    for i, j in ti.ndrange(x_range, y_range):
        I = ti.Vector([i, j])
        dist = (I - point).norm()
        if dist <= rad:
            pixels[I] = color

@ti.func
def draw_line(p1, p2, rad, color):
    x_range = (ti.max(0, int(ti.min(p1.x, p2.x) - rad - 1)), ti.min(int(ti.max(p1.x, p2.y) + rad + 1), res[0]))
    y_range = (ti.max(0, int(ti.min(p1.y, p2.y) - rad - 1)), ti.min(int(ti.max(p1.y, p2.y) + rad + 1), res[1]))
    for I in ti.grouped(ti.ndrange(x_range, y_range)):
        p1I= I - p1
        p12norm = (p2 - p1).normalized()
        dist2 = (p1I - p1I.dot(p12norm) * p12norm).norm()
        dist = ti.min((I - p1).norm(), (I - p2).norm())
        if 0 < p1I.dot(p12norm) < (p2 - p1).norm():
            dist = dist2
        if dist < rad:
            pixels[I] = color

@ti.kernel
def init():
    for I in ti.grouped(pos):
        pos[I] = (I + rand3()) * grid_length 
        pos[I].z += z_near 
        colors[I] = rand3() + ti.Vector([1., 2., 3.])
    



@ti.kernel
def step():
    for i, j, k in ti.ndrange((0, N[0]), (0, N[1]), (0, N[2])):
        I = ti.Vector([i, j, k])
        pos[I].z += velocity[None].z * dt
        # pos[I].z += -1000 * dt
        if pos[I].z < z_near:
            pos[I].z += z_far - z_near

@ti.kernel
def paint():
    for i, j in pixels:
        pixels[i, j] = [0., 0., 0.]
        
    for i, j, k in ti.ndrange((0, N[0]), (0, N[1]), (0, N[2])):
        I = ti.Vector([i, j, k])
        pos_cur, pos_fore = pos[I], pos[I] - velocity[None] * dt
        rad = 3.0 * (1.0 - pos[I].z / z_far) ** 2 # key
        col = colors[I] * (1 - pos_cur.z / z_far) ** 0.5
        col *= 1.0 - (2 * ((pos_cur.x / res[0] - 0.5) ** 2 + (pos_cur.y / res[1] - 0.5) ** 2)) ** 0.2
        # draw_circle(project(pos[I]), rad, colors[I])
        draw_line(project(pos_cur), project(pos_fore), rad, col)
    
# main
init()
t = 0.0
while gui.running:
    t += dt
    velocity[None].z = -20 - (1 - math.cos(t * math.pi / 2)) * 1000
    step()
    paint()
    gui.set_image(pixels)
    gui.show()


