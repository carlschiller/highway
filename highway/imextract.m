C = imread('mall2.png');

countour = zeros(600,550);

for y=1:600
    for x=1:550

        if(C(y,x,2) < 80 )
            countour(y,x) = 1;
        end

        
    end
end

imshow(countour)

dlmwrite('kontur.txt', countour);