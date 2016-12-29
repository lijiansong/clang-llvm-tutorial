

int main(void)
{
    int i = 5;
    int j = 5 + i;
    j = i + 5;
    int k = i + 5;
    int l = 5 + i;
    int m = 6 + i;
    int n = 7 - i;
    int o = i - 7;
    int p = 10 * i;
    int q = i * 10;
    int r = i / 10;
    int s = 10 / i;
    int t = i % 10;
    int u = 10 % i;

    if (i == 10) {
        j = 5;
    }
    else {
        j = 10;
    }

    if (i == 5) {
        j = 5;
    }
    else {
        j = 5;
    }

    while (i < 10) {
        if (i == 10) {
            k = 15;
        }
        else {
            m = 10;
        }
    }

    return i;
}
