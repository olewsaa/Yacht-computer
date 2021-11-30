import time
import dateutil.parser
import matplotlib.pyplot as plt
from influxdb import InfluxDBClient

client = InfluxDBClient(host='localhost', port=8086)
client.switch_database('boatdata')

results=client.query('select mean("value") AS "mean_value" from "environment.outside.pressure" WHERE time > now()-24h group by time(30m) fill(none)')

#print(results.raw)
dx={}
dy={}
points = results.get_points()

j=0
for item in points:
#	print(item['time'])
	ts=item['time']  # Keep the last timestamp, this is now, goes on the header of plot.
	dt = dateutil.parser.parse(item['time'])
#	print(int(time.mktime(dt.timetuple())))
	dx[j]=int(time.mktime(dt.timetuple()))
#	print(item['mean_value'])
	dy[j]=item['mean_value']
	j=j+1

#print(ts)
j=j-1
#print(len(dx))
x=[0]*j
y=[0]*j
#print(j)
for i in range(j):
#	print(j,i,dx[i])
	x[i]=(dx[i]-dx[j])/3600
	y[i]=dy[i]/100+17 # Kjelsås 155moh P=P0*exp(-155/8300)
#	print(x[i])
#	print(y[i])

plt.plot(x,y,'bo')
plt.xlabel('hours  bp')
plt.ylabel('Pressure [hPa]')
plt.title('Barogram at '+ts[:-4].replace('T',' ')+' UTC')
plt.savefig('barogram.png')
plt.show()

