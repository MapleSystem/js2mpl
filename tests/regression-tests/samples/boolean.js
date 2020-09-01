x = (true && true);
y = (false || true);
if (x != true) $ERROR("failed, expect true but get", x);
if (y != true) $ERROR("failed, expect true but get", y);
print("pass");
