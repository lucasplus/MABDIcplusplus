import bpy

# the cube primitive starts as 2m X 2m X 2m by default 

"""

# _________________________________________________________ table

# adding the legs
bpy.ops.mesh.primitive_cube_add(location=( (1-(.1/2)), .5, (.5-(.1/2)) ))
bpy.ops.transform.resize(value=(.1/2, 1/2, .1/2))
bpy.ops.mesh.primitive_cube_add(location=( -(1-(.1/2)), .5, (.5-(.1/2)) ))
bpy.ops.transform.resize(value=(.1/2, 1/2, .1/2))
bpy.ops.mesh.primitive_cube_add(location=( (1-(.1/2)), .5, -(.5-(.1/2)) ))
bpy.ops.transform.resize(value=(.1/2, 1/2, .1/2))
bpy.ops.mesh.primitive_cube_add(location=(-(1-(.1/2)), .5, -(.5-(.1/2)) ))
bpy.ops.transform.resize(value=(.1/2, 1/2, .1/2))

# adding the table top
bpy.ops.mesh.primitive_cube_add(location=( 0, 1-(.1/2), 0 ))
bpy.ops.transform.resize(value=(2/2, .1/2, 1/2))

# _________________________________________________________ cups

# left cup
bpy.ops.mesh.primitive_cylinder_add(location=(.5, 1+(.2/2), 0), rotation=(3.14159265359/2,0.0,0.0))
bpy.ops.transform.resize(value=(0.1/2, .2/2, 0.1/2))

# right cup
bpy.ops.mesh.primitive_cylinder_add(location=(-.5, 1+(.2/2), 0), rotation=(3.14159265359/2,0.0,0.0))
bpy.ops.transform.resize(value=(0.1/2, .2/2, 0.1/2))

"""

# _________________________________________________________ floor

# adding the table top
bpy.ops.mesh.primitive_cube_add(location=( 0, -(.05/2), 0 ))
bpy.ops.transform.resize(value=(20/2, .05/2, 20/2))


# need this so that .ply file gets saved correctly
bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)

# combine into one object
#bpy.ops.object.select_all(action='TOGGLE')
#bpy.ops.object.join()


