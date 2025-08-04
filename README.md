# update-grub
`update-grub` command for distros that don't come with it.

You need:
```
git make gcc sudo install strip
```
and also `grub` but why would you use this if you did not?

Then clone it:
```
git clone https://github.com/bluMATRIKZ/update-grub.git cd update-grub
```

Then compile and install it
```
sudo make
```

And there you go you can run `update-grub` when you please.

And then for removing it you just run:
```
sudo make remove
```
In the project's root.
