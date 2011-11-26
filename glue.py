#!/usr/bin/python

import subprocess,os

working_directory=os.getcwd()

if True:
    loops=open(working_directory+'/loops.csv','w')
    command=[working_directory+'/geometry/geometry_generator', '--radius', '6', '--loops', '113']
    print command
    subprocess.check_call(command,stdout=loops)
    loops.close()

    mesh=open(working_directory+'/mesh.csv','w')
    command=[working_directory+'/mesh/mesh_generator', '--xmax', '9', '--xdiv', '111', '--zdiv', '111', '--zmax', '2']
    print command
    subprocess.check_call(command,stdout=mesh)
    mesh.close()

    command=[working_directory+'/currentLoop']
    print command
    subprocess.check_call(command)


field=working_directory+'/field.csv'
geometry=working_directory+'/loops.csv'
from paraview.simple import *

reader=OpenDataFile(field)
background=OpenDataFile(geometry)

filter=TableToPoints(reader)
filter.XColumn='x'
filter.YColumn='y'
filter.ZColumn='z'
filter.UpdatePipeline()

background_filter=TableToPoints(background)
background_filter.XColumn='R'
background_filter.YColumn='y'
background_filter.ZColumn='Z'
background_filter.UpdatePipeline()

vector=Calculator(filter)
vector.ResultArrayName='B'
vector.Function='Bx*iHat+By*jHat+Bz*kHat'
vector.UpdatePipeline()

streams=StreamTracer(vector, SeedType="High Resolution Line Source" )
streams.Vectors = ['POINTS', 'B']

streams.MaximumError = 1.0e-02
streams.InitialStepLength = 0.1
streams.MaximumStepLength = 0.4
streams.MaximumSteps = 20000
streams.MinimumStepLength = 0.02
streams.MaximumStreamlineLength = 8.99
streams.TerminalSpeed = 9.99e-12

streams.SeedType = "High Resolution Line Source"
streams.SeedType.Point1 = [8.99, 0.0,  0.01]
streams.SeedType.Point2 = [0.01, 0.0, -0.01]
streams.SeedType.Resolution = 100

Show()

view=Render()
view.ViewSize=[400,400]
view.UseLight=1
view.CameraViewUp=[0.0, 0.0, 1.0]
view.CameraPosition=[2.9700000000000002, -12.09592162094027, 0.0]
view.CameraFocalPoint=[2.9700000000000002, 0.0, 0.0]

dp=GetDisplayProperties(vector)
dp.ColorArrayName='B'
lut = GetLookupTableForArray("B", 3, RGBPoints=[6.0567334430367652e-10, 0.23000000000000001, 0.29899999999999999, 0.754, 3.4436464684981819e-05, 0.70599999999999996, 0.016, 0.14999999999999999], VectorMode='Magnitude', ColorSpace='Diverging', UseLogScale=1, ScalarRangeInitialized=1.0)

dp.LookupTable=lut
dp.ColorAttributeType='POINT_DATA'
view.StillRender()

