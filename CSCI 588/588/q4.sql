select F.Name as "SPOUSE1", F2.Name as "SPOUSE2", F.Dept
from Faculty F, Faculty F2
where F.SpouseID = F2.ID AND F.Dept = F2.Dept AND F.ID < F2.ID;
exit
