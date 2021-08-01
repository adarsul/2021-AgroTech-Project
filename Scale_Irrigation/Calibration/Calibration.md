# Scale Calibration


When we started getting readings from the HX711 they were in seemingly arbitrary units. Looking for a way to fix that we came across [this tutorial](https://www.teachmemicro.com/how-to-use-load-cell-hx711-arduino/). We found that the values they used did not work for our scale. Here is a description of what we did to calibrate our scale.

In order to convert these readings to usable values in grams we used a linear regression:

$$ y_{grams} = \alpha * x_{reading} + \beta $$

Using standardized weights and this code we produced 5 readings every second for 10 minutes (roughly 3000 values for each weight). We copied our results to excel and organized in a table:



![Raw%20reading%20head.png](attachment:Raw%20reading%20head.png)


we collected all of the data to one cloumn and created a graph:

![Raw%20Full%20plot.png](attachment:Raw%20Full%20plot.png)

Quite easily one can see that although the values flunctuate, they have certain areas matching their values. To create the linear regression we cleaned the data and calculated the average reading for each weight: 
![Mean%20Reading%20table.png](attachment:Mean%20Reading%20table.png)

Now we create the plot line to find the linear equation:
    ![Linear%20plot.png](attachment:Linear%20plot.png)

so we found that:
    $ \alpha = -0.0098 $
    $ \beta = -531.42 $

Therefore, our linear eqaution is:
$$ y_{grams} =  -0.0098* x_{reading} -531.42 $$

Once we used the equation on the cleaned data we recieved the following table and plot line:

![post%20mean%20table.png](attachment:post%20mean%20table.png)

![Calibrated%20Full%20plot.png](attachment:Calibrated%20Full%20plot.png)

For our project we have used the slope $\alpha$ and used a diffrent value for $\beta$.
When we used heavier subjects we got more accurate results using a different value (compared to our kitchen scale).
In order to find good intercept values we used [this function](https://github.com/adarsul/2021-AgroTech-Project/blob/003237e20f0bff8e47eca04bfc67e6ae48893570/Scale_Irrigation/Calibration/tareIt_Function.ino).


```python

```
