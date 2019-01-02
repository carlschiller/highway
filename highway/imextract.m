C = imread('mall3.png');

countour = zeros(600,550);

for y=1:600
    for x=1:550
        
        if(C(y,x,2) > 200 && C(y,x,1) < 80)
            countour(y,x) = 3;
        end
        if(C(y,x,3) > 200 && C(y,x,1) < 80)
            countour(y,x) = 2;
        end
        if(C(y,x,1) > 200 && C(y,x,2) < 80)
            countour(y,x) = 1;
        end

        
    end
end

thickness = zeros(600,550);

for y=1:600
    for x=1:550
        if(countour(y,x) == 2)
            thickness(y,x) = 1;
            
        end
    end
end

imshow(countour)

dlmwrite('kontur2.txt', countour);
