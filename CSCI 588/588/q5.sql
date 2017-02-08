select Dept, COUNT(ID)-COUNT(SpouseID) as "Single Faculty"
from Faculty
GROUP BY DEPT;
exit
