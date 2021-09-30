package se.nikit.lab2;

public class ResponseData {
    private String htmlTable;
    private boolean hit;
    private String newPageLink;

    public String getHtmlTable() {
        return htmlTable;
    }

    public String getNewPageLink() {
        return newPageLink;
    }

    public boolean isHit() {
        return hit;
    }

    public static Builder newBuilder(){
        return new ResponseData().new Builder();
    }

    public class Builder {

        private Builder() {
        //private builder constructor
        }
        public Builder setHtmlTable(String htmlTable){
            ResponseData.this.htmlTable = htmlTable;
            return this;
        }
        public Builder setHit(boolean hit){
            ResponseData.this.hit = hit;
            return this;
        }
        public Builder setNewPageLink(String newPageLink){
            ResponseData.this.newPageLink = newPageLink;
            return this;
        }
        public ResponseData build(){
            return ResponseData.this;
        }
    }
}
