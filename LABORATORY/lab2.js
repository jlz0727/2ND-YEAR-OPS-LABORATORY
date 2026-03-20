const readline = require("readline-sync");

let ms = parseInt(readline.question("Enter the total memory available (in Bytes) -- "));
let bs = parseInt(readline.question("Enter the block size (in Bytes) -- "));

let nob = Math.floor(ms / bs);
let ef = ms - nob * bs;

let n = parseInt(readline.question("\nEnter the number of processes -- "));

let mp = [];
let tif = 0;
let p = 0;

for (let i = 0; i < n; i++) {
    mp[i] = parseInt(
        readline.question(`Enter memory required for process ${i + 1} (in Bytes)-- `)
    );
}

console.log(`\nNo. of Blocks available in memory--${nob}`);
console.log("\nPROCESS\tMEMORYREQUIRED\tALLOCATED\tINTERNAL FRAGMENTATION");

let i;
for (i = 0; i < n && p < nob; i++) {
    process.stdout.write(`\n ${i + 1}\t\t${mp[i]}`);

    if (mp[i] > bs) {
        console.log("\t\tNO\t\t---");
    } else {
        console.log(`\t\tYES\t${bs - mp[i]}`);
        tif += bs - mp[i];
        p++;
    }
}

if (i < n) {
    console.log("\nMemory is Full, Remaining Processes cannot be accomodated");
}

console.log(`\n\nTotal Internal Fragmentation is ${tif}`);
console.log(`Total External Fragmentation is ${ef}`);