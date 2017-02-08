select distinct F.Name as Faculty, F2.Name as Spouse, 
       T.Dept as Dept, T.Coursenum as Coursenum, T.Sem as Sem
From Student S, Faculty F, Faculty F2, Enrolled E, Taught T
Where E.StudentID = F.SpouseID AND 
      F2.ID = F.SpouseID AND
      F.ID = T.FacultyID AND 
      T.CourseNum = E.CourseNum AND 
      T.Sem = E.Sem AND
      T.Dept = E.Dept
UNION
select distinct F.name as Faculty, S.Name as Spouse,
       T.Dept as Dept, T.Coursenum as Coursenum, T.Sem as Sem
From Student S, Faculty F, Enrolled E, Taught T
Where E.StudentID = F.SpouseID AND
      S.ID = F.SpouseID AND
      F.ID = T.FacultyID AND
      T.CourseNum = E.CourseNum AND
      T.Sem = E.Sem AND
      T.Dept = E.Dept;
exit

