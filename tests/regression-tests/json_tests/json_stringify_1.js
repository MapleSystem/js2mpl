function testcase() {
        try {
                var o = JSON.stringify;
                var desc;
                try {
                        desc = Object.getOwnPropertyDescriptor(JSON, 'stringify')
                } 
                catch (e) {
                };
                var d = delete JSON.stringify;
                if (d === true && JSON.stringify === undefined) {
                        return true;
                }
        } finally {
                if (desc) Object.defineProperty(JSON, 'stringify', desc)
                else JSON.stringify = o  /* this branch messes up the attributes */;
        }
}
testcase()
