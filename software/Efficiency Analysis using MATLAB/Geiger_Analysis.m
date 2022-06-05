clc;
format long;

%% Readings from the Excel, filled with data received on the ESP32 in the time range of 15 minutes
Geiger_Raw_Table = readtable('Geiger_Data.xlsx','Sheet','Sheet1','Range','A1:B42');
Geiger_Raw = table2array(Geiger_Raw_Table);

%giving matlab data from columns
Geiger_without_sample=Geiger_Raw(:,1);
Geiger_Uran238=Geiger_Raw(:,2);

% trimming columns using trimRows function
Geiger_without_sample=trimRows(Geiger_without_sample);
Geiger_Uran238=trimRows(Geiger_Uran238);

%% Theoretical Values
r=[0 0.15]; %meters
P=1.23e-6; %Curies considering that the glass we tested is 185 gr and uranium coloring is 2% of the glass.
countsPerCi = 4.468e10; %counts/s/Ci
flux = P*countsPerCi./(4.*pi.*(r+(1./sqrt(4.*pi))).^2); % counts/s/m^2
Geiger_Area = 0.01*0.1;
Geiger_flux = 60*flux*Geiger_Area; %counts/m

%% Analysis
Detected_Avg_Geiger=[mean(Geiger_without_sample), mean(Geiger_Uran238)]; %averages
Detected_STD_Geiger=[std(Geiger_without_sample), std(Geiger_Uran238)]; %standart deviations

error_Geiger=[Detected_Avg_Geiger-2.*Detected_STD_Geiger;Detected_Avg_Geiger+2.*Detected_STD_Geiger];

%% Theoretical Values with Poisson Correction
Geiger_DeadTime = 10e-4; %s dead time
Geiger_flux_per_DeadTime = Geiger_flux*(1/60)*Geiger_DeadTime; % lambda 02, flux per dead time

%% Real Data Analysis
Error_Correction=sqrt(2*(Detected_STD_Geiger(2)).^2+2*(Detected_STD_Geiger(1)).^2);
Difference_Correction=(Detected_Avg_Geiger(2)-Detected_Avg_Geiger(1));
Geiger_Corrected_15cm=[Difference_Correction + Error_Correction; Difference_Correction-Error_Correction]; %with sample-without sample
alpha=Difference_Correction./Geiger_flux(2);
Error_alpha=abs(alpha).*sqrt((Difference_Correction./Error_Correction).^2+(2*sqrt(Geiger_flux(2))./Geiger_flux(2)).^2);
alpha_95_confidence=[alpha+ Error_alpha;alpha - Error_alpha]

nbins=15;
subplot(2,1,1)
histogram(Geiger_Uran238,nbins)
title("Sample: Uran-238")
xlim([0 60])
subplot(2,1,2)
histogram(Geiger_without_sample,nbins)
title("Without sample")
xlim([0 60])