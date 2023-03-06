
void macro(){
TCanvas *Ls = new TCanvas("Ls","test",500,200);
   TText  t;
   t.SetTextAlign(32);
   t.SetTextSize(0.08);
   Int_t i=1;
   for (float s=0.1; s<1.0 ; s+=0.092) {
      TLine *lh = new TLine(0.15,s,.85,s);
      lh->SetLineStyle(i);
      t.DrawText(0.1,s,Form("%d",i++));
      lh->Draw();
   }
}
