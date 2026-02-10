void func(double *qa)
{
  for(int i = 0; i < 10; i++)
    cout << qa[i] << endl;
}

void test2()
{

  double qa[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  func(&qa[0]);
}
