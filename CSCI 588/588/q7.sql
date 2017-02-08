select F.ID, F.Name, F.Dept, 
       Avg(E.GradeEarned) as "Assigned GPA"
from Faculty F, Enrolled E, Taught T
Where F.ID = T.FacultyID AND
      E.CourseNum = T.Coursenum AND
      E.Dept = T.Dept AND
      E.Sem = T.Sem
group by F.ID, F.Name, F.Dept
order by "Assigned GPA" Desc;
exit
