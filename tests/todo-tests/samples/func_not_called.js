var notcalled = true;

function callback()
{
  notcalled = false;
}

expect = true;
tt = true && true;
actual = [].every(callback) && notcalled;

if (tt != expect) $ERROR("failed, expect true but get", tt);
if (actual != expect) $ERROR("failed, expect true but get", actual);
print("pass");
