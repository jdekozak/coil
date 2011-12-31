#!/usr/bin/python

import subprocess,os

working_directory=os.getcwd()

if True:
    loops=open(working_directory+'/loops.csv','w')
    command=[working_directory+'/geometry/geometry_generator', '--radius', '6', '--loops', '33', '--nef_profile']
    print command
    subprocess.check_call(command,stdout=loops)
    loops.close()

    mesh=open(working_directory+'/mesh.csv','w')
    command=[working_directory+'/mesh/mesh_generator', '--xmax', '12', '--xdiv', '111', '--zdiv', '111', '--zmax', '6']
    print command
    subprocess.check_call(command,stdout=mesh)
    mesh.close()

    command=[working_directory+'/currentLoop']
    print command
    subprocess.check_call(command)


field=working_directory+'/field.csv'
geometry=working_directory+'/loops.csv'
from paraview.simple import *

if True:
    background=OpenDataFile(geometry)

    background_filter=TableToPoints(background)
    background_filter.XColumn='R'
    background_filter.YColumn='y'
    background_filter.ZColumn='Z'
    background_filter.UpdatePipeline()

reader=OpenDataFile(field)
filter=TableToPoints(reader)
filter.XColumn='x'
filter.YColumn='y'
filter.ZColumn='z'
filter.UpdatePipeline()

vector=Calculator(filter)
vector.ResultArrayName='B'
vector.Function='Bx*iHat+By*jHat+Bz*kHat'
vector.UpdatePipeline()

if True:
    glyph=Glyph(vector, GlyphType='Arrow')
    glyph.Vectors = ['POINTS', 'B']
    glyph.SetScaleFactor = 50000.0


if True:
    streams=StreamTracer(vector, SeedType="High Resolution Line Source" )
    streams.Vectors = ['POINTS', 'B']

    streams.MaximumError = 1.0e-02
    streams.InitialStepLength = 0.1
    streams.MaximumStepLength = 0.5
    streams.MaximumSteps = 20000
    streams.MinimumStepLength = 0.002
    streams.MaximumStreamlineLength = 8.99
    streams.TerminalSpeed = 1e-30

    streams.SeedType = "High Resolution Line Source"
    streams.SeedType.Point1 = [8.99, 0.0,  0.01]
    streams.SeedType.Point2 = [0.01, 0.0, -0.01]
    streams.SeedType.Resolution = 100
    streams.UpdatePipeline()

Show()

view=Render()
view.ViewSize=[400,400]
view.UseLight=1
view.CameraViewUp=[0.0, 0.0, 1.0]
view.CameraPosition=[2.97, -12.0959, 0.0]
view.CameraFocalPoint=[2.97, 0.0, 0.0]

dp=GetDisplayProperties(vector)
dp.ColorArrayName='B'
lut = GetLookupTableForArray('B', 3,RGBPoints=[5.5806518400631296e-07,
                                               0.23000000000000001,
                                               0.29899999999999999,
                                               0.754,
                                               8.9029364504078089e-05,
                                               0.70599999999999996,
                                               0.016,
                                               0.14999999999999999],
                             VectorMode='Magnitude', ColorSpace='Diverging', UseLogScale=1, ScalarRangeInitialized=1.0)

dp.LookupTable=lut
dp.ColorAttributeType='POINT_DATA'

dp1=GetDisplayProperties(glyph)
dp1.ColorArrayName='GlyphVector'
lut1 = GetLookupTableForArray('GlyphVector', 3,RGBPoints=[5.5806518400631296e-07,
                                                          0.23000000000000001,
                                                          0.29899999999999999,
                                                          0.754,
                                                          8.9029364504078089e-05,
                                                          0.70599999999999996,
                                                          0.016,
                                                          0.14999999999999999],
                              VectorMode='Magnitude', ColorSpace='Diverging', UseLogScale=1, ScalarRangeInitialized=1.0)
dp1.LookupTable=lut1
dp1.ColorAttributeType='POINT_DATA'


view.StillRender()

