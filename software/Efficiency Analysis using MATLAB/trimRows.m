function out=trimRows(rowMat)
 %trims NaN of column vector
 out=rowMat;
 for i=1:length(rowMat)
 if isnan(rowMat(i))
 out(i:end)=[];
 break
 end
 end
end