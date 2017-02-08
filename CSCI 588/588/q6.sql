select F.Dept, F.name as "FACULTY", S.Name as "STUDENT"
from Faculty F, Student S
where
    F.SpouseID = S.ID;
exit

