close all;
clc;

delimiterIn = ' ';
path = "cmake-build-debug/";
ext = "steps600000.txt";
ext2 = "steps60000ramp.txt";
file30 = importdata("cmake-build-debug/30steps600000.txt",delimiterIn);

strcut = cell(1,20);
strcutramp = cell(1,20);
for i=1:20
    if(i < 10)
        substr = strcat("0",num2str(i));
    else
        substr = num2str(i);
    end
    filename = strcat(strcat(path,substr),ext);
    filename2= strcat(strcat(path,substr),ext2);
    strcut{1,i} = importdata(filename,delimiterIn);
    strcutramp{1,i} = importdata(filename2,delimiterIn);
end

sajs = size(strcut{1,20});

time = linspace(1,10000,sajs(1,1));

means = zeros(1,20);
stds = zeros(1,20);
alphas = linspace(0.1,2,20);

meansramp = zeros(1,20);
stdsramp = zeros(1,20);

for i=1:20
    means(1,i) = mean(strcut{1,i}(1:60000,1));
    meansramp(1,i) = mean(strcutramp{1,i});
    stds(1,i) = std(strcut{1,i}(1:60000,1));
    stdsramp(1,i) = std(strcutramp{1,i});
end

covariance_matrix = cov(means,meansramp);
corr_coeff = covariance_matrix(1,2)/(std(means)*std(meansramp))

p = ranksum(means,meansramp);
p1 = ranksum(strcut{1,10}(1:60000,1),strcutramp{1,10})
%% plots

% figure
% e = errorbar(alphas,means,stds,'*');
% hold on
% errorbar(alphas,meansramp,stdsramp,'*');
% hold off
% axis([0 2 0 1.2])
% lgd = legend({"Ramp meter off","Ramp meter on"},'Interpreter','latex');
% lgd.FontSize = 12;
% tit = title("Flow rate averages",'Interpreter','latex');
% tit.FontSize = 16;
% xl = xlabel("$\alpha$",'Interpreter','latex');
% xl.FontSize = 14;
% yl = ylabel("Flow",'Interpreter','latex');
% yl.FontSize = 14;
% %print(gcf,'fig1.png','-dpng','-r300')
% 
% figure
% plot(time,strcut{1,1})
% hold on
% plot(time,strcut{1,10})
% plot(time,strcut{1,20})
% hold off
% lgd2 = legend({"$\alpha$ = 0.1","$\alpha$ = 1.0","$\alpha$ = 2.0"},'Interpreter','latex');
% lgd2.FontSize = 16;
% 
% tit2 = title("Flow rate over time",'Interpreter','latex');
% tit2.FontSize = 16;
% xl2 = xlabel("time (s)",'Interpreter','latex');
% xl2.FontSize = 14;
% yl2 = ylabel("Flow",'Interpreter','latex');
% yl2.FontSize = 14;
%print(gcf,'fig2.png','-dpng','-r300')
% 
% figure
% histogram(strcut{1,1}(1:60000,1))
% hold on 
% histogram(strcutramp{1,1})
% %histogram(strcut{1,20})
% hold off
% lgd2 = legend({"no ramp meter","ramp meter"},'Interpreter','latex','Location','northwest');
% lgd2.FontSize = 16;
% 
% tit2 = title("Flow rate $\alpha=0.1$",'Interpreter','latex');
% tit2.FontSize = 16;
% xl2 = xlabel("Flow",'Interpreter','latex');
% xl2.FontSize = 14;
% yl2 = ylabel("Number of occurances",'Interpreter','latex');
% yl2.FontSize = 14;
% print(gcf,'fig5.png','-dpng','-r300')

% 
% mean1 = mean(flow);
% mean2 = mean(flow2);
% var1 = std(flow);
% var2 = std(flow2);
% alpha1 = 2;
% alpha2 = 1.4;
% 
% figure
% plot(time,flow)
% hold on
% plot(time,flow2)
% 
% figure
% e = errorbar([alpha1 alpha2],[mean1 mean2],[var1 var2],'*');
% axis([0 3 0 1])